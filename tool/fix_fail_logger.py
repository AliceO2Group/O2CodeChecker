#!/usr/bin/env python ===------------------------------------------------------------------------===#
#
# This script is used for parsing the dump from O2codecheck checks. When an error is dumped, it is translated
# to a key-value pair that accordingly correspond to the problematic part that could not be fixed and the
# string with which the part should be replaced.
#
#===------------------------------------------------------------------------===#
#
import sys
import re
import os

# Reads the .clang_tidy file and imports keys into a set
def read_clang_tidy_file(clang_tidy_file):
  check_error_names_set = set(())

  check_name = ''
  error_name = ''
  with open(clang_tidy_file, 'r') as f:
    for line in f:
      if 'key: ' in line:
        m = re.search('key: \'([^\.]*)\.(.+)\'', line)
        if not m:
          continue
        check_name = m.group(1)
        error_name = m.group(2)
        check_error_names_set.add((check_name, error_name))

  return check_error_names_set

def log_error_names(clang_tidy_output, clang_tidy_file):
  check_error_names_set = read_clang_tidy_file(clang_tidy_file)

  number_of_errors = 0
  for line in clang_tidy_output.split('\n'):
    if 'Could not fix ' not in line:
      continue

    m = re.search('.*Could not fix \'(.+)\'.*\[(.+)\].*', line)
    error_name = m.group(1)
    check_name = m.group(2)

    if( (check_name, error_name) in check_error_names_set ):
      continue
    check_error_names_set.add((check_name, error_name))

    with open(clang_tidy_file, 'a') as f:
      f.write("  - key: '%s.%s'\n    value: ''\n" % (check_name, error_name))
      number_of_errors = number_of_errors + 1

  print 'Number of errors logged:', number_of_errors

def main():
  if len(sys.argv)!=2:
    print 'Provide one argument with path to the .clang-tidy file where the tool will log errors'
    exit(1)

  clang_tidy_file = os.path.join(sys.argv[1], '.clang-tidy')

  if not os.path.isfile(clang_tidy_file):
    print 'Invalid .clang-tidy file:', clang_tidy_file
    exit(2)

  clang_tidy_output = ''

  for line in sys.stdin:
    clang_tidy_output += line

  log_error_names(clang_tidy_output, clang_tidy_file)

if __name__ == '__main__':
  main()
