repl = {"Ook. Ook?": ">",
	"Ook? Ook.": "<",
	"Ook. Ook.": "+",
	"Ook! Ook!": "-",
	"Ook! Ook.": ".",
	"Ook. Ook!": ",",
	"Ook? Ook!": "]",
	"Ook! Ook?": "["}

import re
import sys
ook = sys.argv[1]
print(re.sub(r'[A-Za-z]+[.!?] [A-Za-z]+[.!?]', lambda m: repl[m.group(0)], ook).replace(' ', ''))
