repl = {"Ook. Ook?": ">",
	"Ook? Ook.": "<",
	"Ook. Ook.": "+",
	"Ook! Ook!": "-",
	"Ook! Ook.": ".",
	"Ook. Ook!": ",",
	"Ook? Ook!": "]",
	"Ook! Ook?": "["}

repl = dict([(y, x) for x,y in repl.items()])

import re
import sys
ook = sys.argv[1]

print(re.sub(r'.', lambda m: repl[m.group(0)], ook))
