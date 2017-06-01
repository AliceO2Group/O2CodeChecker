#!/usr/bin/python

# A python script with the goal
# to transform a (large) compilations database
# to a smaller compilations database, given a list of source files
# that have changed (currently only supporting c++ source and header files)
# This should be useful to reduce the time spent in static code analysis by only
# checking files that have changed, or are influenced by a change
#
# First version: Sandro Wenzel (June 2017)

import json
import os
import subprocess
import re
# these are for queue syncronized multi-threading
import Queue
import threading
import multiprocessing
import time
import sys

# this is a list of changed files we get from git (or the pull request)
listofchangedfiles=['O2Device.cxx', 'CalDet.h', 'CalDet.cxx']
checkall=False

def isHeaderFile(filename):
    # make this more general
    expression=".*\.h"
    # make this more efficient by compiling the expression
    result=re.match(expression, filename)
    if not result == None:
        return True
    return False
    
def isSourceFile(filename):
    # make this more general
    expression=".*\.cxx"
    # make this more efficient by compiling the expression
    result=re.match(expression, filename)
    if not result == None:
        return True
    return False

def isROOTDictionaryFile(filename):
    expression=".*G\_\_.*\.cxx"
    # make this more efficient by compiling the expression
    result=re.match(expression, filename)
    if not result == None:
        return True
    return False

def isProtoBuffFile(filename):
    expression=".*\.pb\.cc"
    # make this more efficient by compiling the expression
    result=re.match(expression, filename)
    if not result == None:
        return True
    return False

def isInvalid(filename):
    return isROOTDictionaryFile(filename) or isProtoBuffFile(filename)

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
    result=re.match(expression, line)
    if not result == None:
        return True
    return False
    
def queryListOfHeaders(command):
    proc=subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    headerlist=[]
    for line in proc.stdout:
        if matchesHeader(line.strip(), headerlist):
            headerlist.append(line.strip())
    return headerlist

# service that processes one item in the queue
def processItem(keepalive, changedheaderlist, queue, outqueue):
  while len(keepalive)>0:
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
                      outqueue.put(entry)
                      continue

          queue.task_done()
      except Queue.Empty:
          pass

def reportProgress(keepalive, queue, q2):
    while len(keepalive)>0:
        print "input queue has size " + str(queue.qsize())
        print "output queue has size " + str(q2.qsize())
        time.sleep(1)

#
# THE MAIN FUNCTION
#
def main():

    #open the compilations database 
    file=open('compile_commands.json').read()
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

    # make a queue
    inputqueue = Queue.Queue()
    outputqueue = Queue.Queue()
    keepAlive=['alive']

    # make some servicing threads
    max_task = multiprocessing.cpu_count()
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
    print "Processing " + str(len(data)) + " items "
    for entry in data:
        filename=entry['file']
        basename=os.path.basename(filename)

        # check if invalid anyway
        if (isInvalid(basename)):
            continue
        
        # check if this entry is part of the changed source file list
        # if yes, continue directly
        if checkall==True or (basename in changedsourcefilelist):
            outputqueue.put(entry)
            continue
    
        # otherwise check if this source file is influenced by some changed header file
        # TODO: if the header does not contain a template, it might be enough
        # to only add one single source files that depends on it????
        inputqueue.put(entry)

    # wait on the queue --> wait until queue is completely empty
    inputqueue.join()
    # now we can shut down the deamon threads
    keepAlive[:]=[]

    #put outputqueue into outputdict
    while outputqueue.qsize()>0:
        outputdict.append(outputqueue.get(False))

    # write result dictionary to json
    outjson = json.dumps(outputdict, sort_keys=True, indent=4, separators=(',', ': '))
    with open("thinned_compile_commands.json",'w') as fp:
        fp.write(outjson)

    return

if __name__ == '__main__':
  main()
