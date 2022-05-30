#!/usr/bin/env python3

import sys
import os

class kex(object):
	''' recursive flush/close stdout/stderr '''
	def __call__(self,e,r):
		try:
			return e[0]()
		except (IOError,BrokenPipeError,KeyboardInterrupt) as x:
			self(e[1:],r+1)
		return r

def main(s):
	if os.geteuid()==os.getuid() and os.getuid():
		print('main',flush=True)
		return 0
	else:
		print('stop running as root/suid',file=sys.stderr)
		return 1

if __name__=='__main__':
	sys.exit(kex()([
		lambda :main(sys.argv),
		sys.stdout.flush,
		sys.stdout.close,
		sys.stderr.flush,
		sys.stderr.close],1))
