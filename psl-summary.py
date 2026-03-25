#!/usr/bin/env python3

import re
import sys

# TODO: best match, see https://github.com/publicsuffix/list/wiki/Format#formal-algorithm

def usage():
    print(f"Usage: $0 public_suffix_list.dat [fqdn_to_match]")
    sys.exit(1)

def exact_match(rule):
    if rule == fqdn_to_match:
        print(f"{rule} exact match found!")

def suffix_match(rule):
    if fqdn_to_match.endswith('.' + rule):
        print(f"{rule} suffix match!")

def exception_match(rule):
    if rule.startswith('!') and fqdn_to_match.endswith(rule[1:]):
        print(f"{rule} exception match!")

def wildcard_match(rule):
    if rule.startswith('*.') and fqdn_to_match.endswith(rule[1:]):
        print(f"{rule} wildcard match!")
 
if len(sys.argv) < 2 or len(sys.argv) > 3:
    usage()

fqdn_to_match = sys.argv[2].lower() if len(sys.argv) == 3 else None
if fqdn_to_match:
    fqdn_to_match = fqdn_to_match.rstrip('.')

num_of_rules = 0
num_of_exceptions = 0
num_of_wildcards = 0
num_of_tlds = 0

try:
    psl = open(sys.argv[1])
except:
    print("Unable to open {sys.argv[1]", file=sys.stderr)
    sys.exit(1)

for line in psl:

    if not line:
        continue

    # WARNING: what if line starts with single '/'?
    # specs do not consider this corner, presumably rare, case
    if line.startswith('//'):
        continue

    # each line read up to first white space
    # therefore, this shouldn't happen, but specs are specs
    # e.g., '^   foo.bar.baz'?
    if line[0].isspace():
        continue

    rule = line.split()[0].lower()
    num_of_rules += 1

    if line.startswith('!'):
        num_of_exceptions += 1

    if line.startswith('*.'):
        num_of_wildcards += 1

    if '.' in line:
        num_of_tlds += 1

    if fqdn_to_match:
        suffix_match(rule)
        exact_match(rule)
        exception_match(rule)
        wildcard_match(rule)

psl.close()

print(f"Number of rules: {num_of_rules}")
print(f"Number of exceptions: {num_of_exceptions}")
print(f"Number of wildcards: {num_of_wildcards}")
print(f"Number of TLDs: {num_of_tlds}")
