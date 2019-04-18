#!/usr/bin/env python

# A python script with the goal
# to transform a (large) compilations database
# to a smaller compilations database, given a list of source files
# that have changed (currently only supporting c++ source and header files).
# This should be useful to reduce the time spent in static code analysis by only
# checking files that have changed, or are influenced by a change.
#
# First version: Sandro Wenzel (June 2017)

from __future__ import print_function
import argparse
import json
import os
import subprocess
import re
# these are for queue syncronized multi-threading
try:
  import Queue
except ImportError:
  import queue as Queue
import threading
import multiprocessing
import time
import sys

verbosity=0

def parseArgs():
    """ Setup + Parse arguments from command line; return a parse object """
    parser = argparse.ArgumentParser(description='Runs over all entries in a '
                                   'compilation database. Produces a filtered/thinned version of it.')
    parser.add_argument('-j', type=int, default=0,
                        help='Number of threads to be used for processing. In any case,'
                              'not more that the number of available CPU threads will be used (which is the default).')
    parser.add_argument('-use-files', default=None,
                        help='\':\' separated file list used to thin out the database. Usually a list of header and source files.'
                            '\n If no files are given, the default behaviour is to run over the complete database and filter out'
                            'only ROOT dictionary files and Protobuf generated files.')
    parser.add_argument('-exclude-files', default=None,
                        help='A regular expression telling which files to exclude from the database. Takes precedence over \'-use-files\'.')
    parser.add_argument('-o', default='thinned_compile_commands.json',
                        help='Filename of output compilations database. [default: thinned_compile_commands.json]')
    parser.add_argument('--verbose', type=int, default=0,
                        help='Display verbose information about what is going on')
    return parser.parse_args()

def verboseLog(string, level=0):
    global verbosity
    if verbosity > 0:
        print(string)

def getListOfChangedFiles(colonseparatedfilepaths):
    """ processes the argument '-use-files' and returns a python list of filenames """
    return colonseparatedfilepaths.split(":")

def makeInvalidClosure(args):
    regex = None
    if args.exclude_files:
      regex=re.compile(args.exclude_files)
    def f(filename):
        return regex.match(filename) is not None if regex else None
    return f

def isHeaderFile(filename):
    # make this more general
    expression=".*\.h"
    # make this more efficient by compiling the expression
    return re.match(expression, filename) is not None
    
def isSourceFile(filename):
    # make this more general
    expression=".*\.cxx"
    # make this more efficient by compiling the expression
    return re.match(expression, filename) is not None

# modifies a compilation command by appending -MM (and removing the -o flag)
# in order to retrieve the header dependencies
def modifyCompileCommand(command):
    newcommand=[]
    outputflagseen=False
    # take out the output command
    for token in command.split(' '):
        if len(token)==0:
            continue
        if token=='-o':
            outputflagseen=True;
        else:
            if not outputflagseen:
                newcommand.append(token)
            else:
                outputflagseen=False
    # add the -MM flag which will generate the header dependency list for a source file
    newcommand.append('-MM')
    return newcommand

def matchesHeader(line, header):
    expression=".*\.h"
    # make this more efficient by compiling the expression
    return re.match(expression, line) is not None
    
def queryListOfHeaders(command):
    proc=subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    headerlist=[]
    for line in proc.stdout:
        if matchesHeader(line.strip(), headerlist):
            headerlist.append(line.strip())
    return headerlist

# service that processes one item in the queue
def processItem(keepalive, changedheaderlist, queue, outqueue):
    while len(keepalive) > 0:
      try:
          # this operation is blocking for at most 0.5 seconds
          # sad hack to be able to check whether we want this thread to be kept alive
          # unfortunately, the main process never exits otherwise
          entry = queue.get(True, 0.5)
          includedheaderlist=queryListOfHeaders(modifyCompileCommand(entry['command']))
          for header in changedheaderlist:
              for include in includedheaderlist:
                  expression=".*"+header
                  matches = re.match(expression, include)
                  if not matches == None:
                      verboseLog("Adding " + entry['file'] + " because of a dependence on a modified header")
                      outqueue.put(entry)
                      continue

          queue.task_done()
      except Queue.Empty:
          pass

def reportProgress(keepalive, queue, q2):
    while len(keepalive)>0:
        print("input queue has size " + str(queue.qsize()))
        print("output queue has size " + str(q2.qsize()))
        time.sleep(1)

#custom function to remove duplicates and return a new list
#set() cannot be used since the entries are not hashable
def removeDuplicates(inputlist):
    outputlist=[]
    for e in inputlist:
        if e not in outputlist:
            outputlist.append(e)
    return outputlist


def fileMatchesAnyInSourceFileList(filename, sourcefilelist):
   '''returns true if any filename in sourcefilelist matches
   filename

   items in sourcefilelist will have only paths relativ (to
   a git repository); while filename is a fully qualified path
   '''
   # TODO: item should be at end of filename
   for item in sourcefilelist:
       if item in filename:
           return True
   return False

#
# THE MAIN FUNCTION
#
def main():
    checkall=False
    listofchangedfiles=[]
    args = parseArgs()
    global verbosity
    verbosity = args.verbose
    if not args.use_files:
        verboseLog("no changeset given ... putting all (modulo the exclude set)")
        checkall=True
    else:
        listofchangedfiles=getListOfChangedFiles(args.use_files)

    #setup the isInvalid (closure) function
    isInvalid=makeInvalidClosure(args)

    #open the compilations database
    try:
        file=open('compile_commands.json').read()
    except IOError:
        print("Problem opening the compilation database (file not found)")
        sys.exit(1)

    #convert json to dict
    data=json.loads(file);

    #make 2 lists, changedheaders and changedsourcefiles
    changedheaderlist=[]
    changedsourcefilelist=[]
    for file in listofchangedfiles:
        if isHeaderFile(file):
            changedheaderlist.append(file)
        elif isSourceFile(file):
            changedsourcefilelist.append(file)

    # make input/output queues for multithreaded processing
    inputqueue = Queue.Queue()
    outputqueue = Queue.Queue()
    keepAlive=['alive']

    # make some servicing threads
    max_task = multiprocessing.cpu_count()
    if args.j:
        if args.j>0:
            max_task = max(max_task,j)

    verboseLog("processing with " + str(max_task) + " threads.")
    for _ in range(max_task):
        t = threading.Thread(target=processItem, args=(keepAlive,changedheaderlist,inputqueue,outputqueue))
        t.deamon=True
        t.start()

    # launch the reporting thread
    rt = threading.Thread(target=reportProgress, args=(keepAlive,inputqueue,outputqueue))
    rt.deamon=True
    rt.start()

    outputdict=[]
    #scan through compile database and filter against files
    verboseLog("Processing " + str(len(data)) + " items.")
    for entry in data:
        filename=entry['file']
        basename=os.path.basename(filename)

        # check if invalid anyway
        if (isInvalid(basename)):
            verboseLog("Excluding " + basename + " because of exclude filter")
            continue
        
        # check if this entry is part of the changed source file list
        # if yes, continue directly
        if checkall==True or fileMatchesAnyInSourceFileList(filename, changedsourcefilelist):
            verboseLog("Adding " + entry['file'] + " because of presence in modify list (or lack thereof)")
            outputqueue.put(entry)
            continue

        if len(changedheaderlist) > 0:
            # otherwise check if this source file is influenced by some changed header file
            # TODO: if the header does not contain a template, it might be enough
            # to only add one single source files that depends on it????
            inputqueue.put(entry)

    # wait on the queue --> wait until queue is completely empty
    inputqueue.join()
    # now we can shut down the deamon threads
    keepAlive[:]=[]

    #put outputqueue into outputdict
    while outputqueue.qsize() > 0:
        outputdict.append(outputqueue.get(False))

    # write result dictionary to json
    outjson = json.dumps(removeDuplicates(outputdict), sort_keys=True, indent=4, separators=(',', ': '))
    with open(args.o,'w') as fp:
        fp.write(outjson)

    return

if __name__ == '__main__':
  main()
