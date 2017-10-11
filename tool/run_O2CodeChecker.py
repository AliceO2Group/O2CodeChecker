#!/usr/bin/env python
#
#===- run-clang-tidy.py - Parallel clang-tidy runner ---------*- python -*--===#
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
#===------------------------------------------------------------------------===#
# FIXME: Integrate with clang-tidy-diff.py

"""
Parallel clang-tidy runner
==========================

Runs clang-tidy over all files in a compilation database. Requires clang-tidy
and clang-apply-replacements in $PATH.

Example invocations.
- Run clang-tidy on all files in the current working directory with a default
  set of checks and show warnings in the cpp files and all project headers.
    run-clang-tidy.py $PWD

- Fix all header guards.
    run-clang-tidy.py -fix -checks=-*,llvm-header-guard

- Fix all header guards included from clang-tidy and header guards
  for clang-tidy headers.
    run-clang-tidy.py -fix -checks=-*,llvm-header-guard extra/clang-tidy \
                      -header-filter=extra/clang-tidy

Compilation database setup:
http://clang.llvm.org/docs/HowToSetupToolingForLLVM.html
"""

import argparse
import json
import multiprocessing
import os
import Queue
import re
import shutil
import subprocess
from subprocess import Popen, PIPE, STDOUT
import sys
import tempfile
import threading


def find_compilation_database(path):
  """Adjusts the directory until a compilation database is found."""
  result = './'
  while not os.path.isfile(os.path.join(result, path)):
    if os.path.realpath(result) == '/':
      print 'Error: could not find compilation database.'
      sys.exit(1)
    result += '../'
  return os.path.realpath(result)


def get_tidy_invocation(f, clang_tidy_binary, checks, warningsAsErrors, tmpdir, build_path,
                        header_filter, config):
  """Gets a command line for clang-tidy."""
  start = [clang_tidy_binary]
  if header_filter is not None:
    start.append('-header-filter=' + header_filter)
  else:
    # Show warnings in all in-project headers by default.
    start.append('-header-filter=^' + build_path + '/.*')
  if checks:
    start.append('-checks=' + checks)
  if warningsAsErrors:
    start.append('-warnings-as-errors=' + warningsAsErrors)
  if config:
    start.append('-config=' + config)
  if tmpdir is not None:
    start.append('-export-fixes')
    # Get a temporary file. We immediately close the handle so clang-tidy can
    # overwrite it.
    (handle, name) = tempfile.mkstemp(suffix='.yaml', dir=tmpdir)
    os.close(handle)
    start.append(name)
  start.append('-p=' + build_path)
  start.append(f)
  return start


def apply_fixes(args, tmpdir):
  """Calls clang-apply-fixes on a given directory. Deletes the dir when done."""
  invocation = [args.clang_apply_replacements_binary]
  if args.format:
    invocation.append('-format')
  invocation.append(tmpdir)
  subprocess.call(invocation)
  shutil.rmtree(tmpdir)


def run_tidy(args, tmpdir, build_path, queue, clang_tidy_output_list):
  """Takes filenames out of queue and runs clang-tidy on them."""
  while True:
    name = queue.get()
    invocation = get_tidy_invocation(name, args.clang_tidy_binary, args.checks, args.warningsAsErrors,
                                     tmpdir, build_path, args.header_filter, args.config)
    sys.stdout.write(' '.join(invocation) + '\n')
    #subprocess.call(invocation)
    p = Popen(invocation, stdout=PIPE, stderr=STDOUT)
    output = p.communicate()[0]
    clang_tidy_output_list.append(output)
    print output

    queue.task_done()


def main():
  parser = argparse.ArgumentParser(description='Runs clang-tidy over all files '
                                   'in a compilation database. Requires '
                                   'clang-tidy and clang-apply-replacements in '
                                   '$PATH.')
  parser.add_argument('-clang-tidy-binary', metavar='PATH',
                      default='O2CodeChecker',
                      help='path to O2CodeChecker binary')
  parser.add_argument('-clang-apply-replacements-binary', metavar='PATH',
                      default='clang-apply-replacements',
                      help='path to clang-apply-replacements binary')
  parser.add_argument('-checks', default=None,
                      help='checks filter, when not specified, use clang-tidy '
                      'default')
  parser.add_argument('-warnings-as-errors', default=None, dest="warningsAsErrors",
                      help='checks which should become errors when triggered '
                      'default')
  parser.add_argument('-config', default=None,
                       help='config option for check , when not specified, use clang-tidy '
                       'default')
  parser.add_argument('-header-filter', default=None,
                      help='regular expression matching the names of the '
                      'headers to output diagnostics from. Diagnostics from '
                      'the main file of each translation unit are always '
                      'displayed.')
  parser.add_argument('-j', type=int, default=0,
                      help='number of tidy instances to be run in parallel.')
  parser.add_argument('files', nargs='*', default=['.*'],
                      help='files to be processed (regex on path)')
  parser.add_argument('-fix', action='store_true', help='apply fix-its')
  parser.add_argument('-format', action='store_true', help='Reformat code '
                      'after applying fixes')
  parser.add_argument('-p', dest='build_path',
                      help='Path used to read a compile command database.')
  parser.add_argument('-clang-tidy-file-path', default=None,
                      help='Path to the .clang-tidy file where the configuration is written. '
                      'Usually this file should be in the main source directory of the project.')
  args = parser.parse_args()

  db_path = 'compile_commands.json'

  if args.build_path is not None:
    build_path = args.build_path
  else:
    # Find our database
    build_path = find_compilation_database(db_path)

  try:
    invocation = [args.clang_tidy_binary, '-list-checks']
    invocation.append('-p=' + build_path)
    if args.config:
      invocation.append('-config=' + args.config)
    if args.checks:
      invocation.append('-checks=' + args.checks)
    invocation.append('-')
    print subprocess.check_output(invocation)
  except:
    print >>sys.stderr, "Unable to run clang-tidy."
    sys.exit(1)

  # Load the database and extract all files.
  database = json.load(open(os.path.join(build_path, db_path)))
  files = [entry['file'] for entry in database]

  max_task = args.j
  if max_task == 0:
    max_task = multiprocessing.cpu_count()

  tmpdir = None
  if args.fix:
    tmpdir = tempfile.mkdtemp()

  # Build up a big regexy filter from all command line arguments.
  file_name_re = re.compile('(' + ')|('.join(args.files) + ')')
  clang_tidy_output_list = []

  try:
    # Spin up a bunch of tidy-launching threads.
    queue = Queue.Queue(max_task)
    for _ in range(max_task):
      t = threading.Thread(target=run_tidy,
                           args=(args, tmpdir, build_path, queue, clang_tidy_output_list))
      t.daemon = True
      t.start()

    # Fill the queue with files.
    for name in files:
      if file_name_re.search(name):
        queue.put(name)

    # Wait for all threads to be done.
    queue.join()

  except KeyboardInterrupt:
    # This is a sad hack. Unfortunately subprocess goes
    # bonkers with ctrl-c and we start forking merrily.
    print '\nCtrl-C detected, goodbye.'
    if args.fix:
      shutil.rmtree(tmpdir)
    os.kill(0, 9)

  if args.fix:
    print 'Applying fixes ...'
    apply_fixes(args, tmpdir)

  if args.clang_tidy_file_path:
    print('Running fix_fail_logger.py')
    fix_fail_logger = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'fix_fail_logger.py')
    clang_tidy_output = '\n'.join(clang_tidy_output_list)

    #p = Popen(['python', './fix_fail_logger.py', clang_tidy_file], stdin=PIPE, stdout=PIPE, stderr=STDOUT)
    p = Popen(['python', fix_fail_logger, args.clang_tidy_file_path], stdin=PIPE, stdout=PIPE, stderr=STDOUT)
    print p.communicate(input=clang_tidy_output)[0]

if __name__ == '__main__':
  main()
