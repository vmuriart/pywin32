"""Exception and error handling.

 This contains the core exceptions that the implementations should raise
 as well as the IActiveScriptError interface code.
 
"""

import sys, string, traceback
from win32com.axscript import axscript
import winerror
import win32com.server.exception
import win32com.server.util
import pythoncom
import re

debugging = 0

def FormatForAX(text):
	"""Format a string suitable for an AX Host
	"""
	# Replace all " with ', so it works OK in HTML (ie, ASP)
	return ExpandTabs(AddCR(text))

def ExpandTabs(text):
	return re.sub('\t','    ', text)

def AddCR(text):
	return re.sub('\n','\r\n',text)
#	return string.join(string.split(text,'\n'),'\r\n')

class IActiveScriptError:
	"""An implementation of IActiveScriptError
	
	The ActiveX Scripting host calls this client whenever we report
	an exception to it.  This interface provides the exception details
	for the host to report to the user.
	"""
	_com_interfaces_ = [axscript.IID_IActiveScriptError]
	_public_methods_ = ["GetSourceLineText","GetSourcePosition","GetExceptionInfo"]
	def _query_interface_(self, iid):
		print "IActiveScriptError QI - unknown IID", iid
		return 0
	def _SetExceptionInfo(self, exc):
		self.exception = exc
	def GetSourceLineText(self):
		return self.exception.linetext
	def GetSourcePosition(self):
		ctx = self.exception.sourceContext
		# Zero based in the debugger (but our columns are too!)
		return ctx, self.exception.lineno + self.exception.startLineNo-1, self.exception.colno
	def GetExceptionInfo(self):
		return self.exception

class AXScriptException(win32com.server.exception.COMException):
	"""A class used as a COM exception.
	
	Note this has attributes which conform to the standard attributes 
	for COM exceptions, plus a few others specific to our IActiveScriptError
	object.
	"""
	def __init__(self, site, codeBlock, exc_type, exc_value, exc_traceback):
		# set properties base class shares via base ctor...
		win32com.server.exception.COMException.__init__( self, \
			description = "Unknown Exception", \
			scode = winerror.DISP_E_EXCEPTION, \
			source = "Python ActiveX Scripting Engine",
			)
			
		# And my other values...
		if codeBlock is None:
			self.sourceContext = 0
			self.startLineNo = 0
		else:
			self.sourceContext = codeBlock.sourceContextCookie
			self.startLineNo = codeBlock.startLineNumber
		self.linetext = ""

		self.__BuildFromException(site, exc_type, exc_value, exc_traceback)

	def __BuildFromException(self, site, type , value, tb):
		if debugging:
			import linecache
			linecache.clearcache()
		try:
			if type is SyntaxError:
				self._BuildFromSyntaxError(site, value, tb)
			else:
				self._BuildFromOther(site, type, value, tb)
		except: # Error extracting traceback info!!!
			traceback.print_exc()
			# re-raise.
			raise sys.exc_info()

	def _BuildFromSyntaxError(self, site, value, tb):
		try:
			msg, (filename, lineno, offset, line) = value
		except:
			msg = "Unknown"
			lineno = 0
			offset = 0
			line = "Unknown"
		self.description=FormatForAX(msg)
		self.lineno = lineno
		self.colno = offset - 1
		self.linetext = ExpandTabs(string.rstrip(line))

	def _BuildFromOther(self, site, type, value, tb):
		self.colno = -1
		self.lineno = 0
		if debugging: # Full traceback if debugging.
			list=traceback.format_exception(type, value, tb)
			self.description = ExpandTabs(string.join(list,""))
			print "Script Traceback is", self.description
			return
		# Run down the traceback list, looking for the first "<Script..>"
		# Hide traceback above this.  In addition, keep going down
		# looking for a "_*_" attribute, and below hide these also.
		hide_names = ["r_import","r_reload","r_open"] # hide from these functions down in the traceback.
		depth = None
		tb_top = tb
		while tb_top:
			filename, lineno, name, line = self.ExtractTracebackInfo(tb_top, site)
			if filename[:7]=="<Script":
				break
			tb_top = tb_top.tb_next
		format_items = []
		if tb_top: # found one.
			depth = 0
			tb_look = tb_top
			# Look down for our bottom
			while tb_look:
				filename, lineno, name, line = self.ExtractTracebackInfo(tb_look, site)
				if name in hide_names:
					break
				self.lineno = lineno
				self.linetext = line
				format_items.append((filename, lineno, name, line))
				depth = depth + 1
				tb_look = tb_look.tb_next
		else:
			depth = None
			tb_top = tb
			
		list = ['Traceback (most recent call last):\n']
		list = list + traceback.format_list(format_items)
		if type==pythoncom.com_error:
			desc = "%s (0x%x)" % (value[1], value[0])
			if value[0]==winerror.DISP_E_EXCEPTION and value[2] and value[2][2]:
				desc = value[2][2]
			list.append("COM Error: "+desc)
		else:
			list = list + traceback.format_exception_only(type, value)
#		list=traceback.format_exception(type, value, tb_top, depth)
		self.description = ExpandTabs(string.join(list,""))
		# Clear tracebacks etc.
		tb = tb_top = tb_look = None
#		sys.exc_type = sys.exc_value = sys.exc_traceback = None

	def ExtractTracebackInfo(self, tb, site):
		import linecache
		f = tb.tb_frame
		lineno = tb.tb_lineno
		co = f.f_code
		filename = co.co_filename
		name = co.co_name
		line = linecache.getline(filename, lineno)
		if not line:
			try:
				codeBlock = site.scriptCodeBlocks[filename]
			except KeyError:
				codeBlock = None
			if codeBlock:
				line = codeBlock.GetLineNo(lineno)
		if line: 
			line = string.strip(line)
		else: 
			line = None
		return filename, lineno, name, line
	def __repr__(self):
		return "AXScriptException Object with description:" + self.description

def ProcessAXScriptException(scriptingSite, debugManager, exceptionInstance):
	"""General function to handle any exception in AX code
	
	This function creates an instance of our IActiveScriptError interface, and
	gives it to the host, along with out exception class.  The host will
	likely call back on the IActiveScriptError interface to get the source text
	and other information not normally in COM exceptions.
	"""
#	traceback.print_exc()
	instance = IActiveScriptError()
	instance._SetExceptionInfo(exceptionInstance)
	gateway = win32com.server.util.wrap(instance, axscript.IID_IActiveScriptError)
	if debugManager:
		fCallOnError = debugManager.HandleRuntimeError()
		if not fCallOnError:
			return None
		
	try:
		result = scriptingSite.OnScriptError(gateway)
	except pythoncom.com_error, details:
		print "**OnScriptError failed:", details
		print "Exception description:'%s'" % (`exceptionInstance.description`)
		print "Exception text:'%s'" % (`exceptionInstance.linetext`)
		result = winerror.S_FALSE

	if result==winerror.S_OK:
		# If the above  returns NOERROR, it is assumed the error has been 
		# correctly registered and the value SCRIPT_E_REPORTED is returned.
		ret = win32com.server.exception.COMException(scode=axscript.SCRIPT_E_REPORTED)
		return ret
	else:
		# The error is taken to be unreported and is propagated up the call stack 
		# via the IDispatch::Invoke's EXCEPINFO parameter (hr returned is DISP_E_EXCEPTION. 
		return exceptionInstance
