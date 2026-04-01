#!/usr/bin/env python3

import locale
import sys

print(
    '''
#!/usr/bin/env python3

"""
vtparse.py -- ansi terminal code parser

ported to python from

        https://github.com/haberman/vtparse

which is based on

      https://vt100.net/emu/dec_ansi_parser
"""

from __future__ import print_function as _

## pylint: disable=invalid-name,too-many-lines

import locale
import sys

__all__ = ("VTParser", "clean", "main")

ENCODING = locale.getpreferredencoding()
ENCODING = "latin-1"
'''.lstrip()
)

for line in sys.stdin:
    if "typedef enum" in line:
        break
else:
    raise RuntimeError(1)


for line in sys.stdin:
    if "vtparse_state_t" in line:
        break
    print(line.strip().rstrip(","))
else:
    raise RuntimeError(2)


for line in sys.stdin:
    if "typedef enum" in line:
        break
else:
    raise RuntimeError(3)


print()
for line in sys.stdin:
    if "vtparse_action_t" in line:
        break
    print(line.strip().rstrip(","))
else:
    raise RuntimeError(4)


print()
for line in sys.stdin:
    if "extern" in line:
        continue
    if "ACTION_NAMES" in line:
        print("ACTION_NAMES = (")
        break
else:
    raise RuntimeError(5)


for line in sys.stdin:
    if "};" in line:
        print(")")
        break
    print("    " + line.strip())
else:
    raise RuntimeError(6)


print()
for line in sys.stdin:
    if "STATE_NAMES" in line:
        print("STATE_NAMES = (")
        break
else:
    raise RuntimeError(7)


for line in sys.stdin:
    if "};" in line:
        print(")")
        break
    print("    " + line.strip())
else:
    raise RuntimeError(8)


print()
for line in sys.stdin:
    if "STATE_TABLE" in line:
        print("STATE_TABLE = (")
        break
else:
    raise RuntimeError(9)


for line in sys.stdin:
    if "};" in line:
        print(")")
        break
    if line.strip() == "{":
        print("    (")
    elif "}," in line:
        print("    ),")
    else:
        print("        " + line.strip())
else:
    raise RuntimeError(10)


print()
for line in sys.stdin:
    if "ENTRY_ACTIONS" in line:
        print("ENTRY_ACTIONS = (")
        break
else:
    raise RuntimeError(11)


for line in sys.stdin:
    if "};" in line:
        print(")")
        break
    print("    " + line.strip().rstrip(",").strip() + ",")
else:
    raise RuntimeError(12)


print()
for line in sys.stdin:
    if "EXIT_ACTIONS" in line:
        print("EXIT_ACTIONS = (")
        break
else:
    raise RuntimeError(13)


for line in sys.stdin:
    if "};" in line:
        print(")")
        break
    print("    " + line.strip().rstrip(",").strip() + ",")
else:
    raise RuntimeError(14)


print()
print()
for line in open(
    "vtparse_gen.inc", "r", encoding=locale.getpreferredencoding()
):
    print(line, end="")


## EOF
