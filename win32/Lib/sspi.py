"""
Helper classes for SSPI authentication via the win32security module.

SSPI authentication involves a token-exchange "dance", the exact details
of which depends on the authentication provider used.  There are also
a number of complex flags and constants that need to be used - in most
cases, there are reasonable defaults.

These classes attempt to hide these details from you until you really need
to know.
"""
# Based on Roger Upole's sspi demos.
# $Id$
import win32security, sspicon

try:
    True, False
except NameError:
    False = 1==0
    True = not False

class _BaseAuth(object):
    def __init__(self):
        self.reset()

    def reset(self):
        self.ctxt = None
        self.authenticated = False

    def encrypt(self, data):
        """Encrypt a string, returning a string suitable for transmission"""
        pkg_size_info=self.ctxt.QueryContextAttributes(sspicon.SECPKG_ATTR_SIZES)
        trailersize=pkg_size_info['SecurityTrailer']

        encbuf=win32security.SecBufferDescType()
        encbuf.append(win32security.SecBufferType(len(data), sspicon.SECBUFFER_DATA))
        encbuf.append(win32security.SecBufferType(trailersize, sspicon.SECBUFFER_TOKEN))
        encbuf[0].Buffer=data
        self.ctxt.EncryptMessage(0,encbuf,1)
        # And return all buffers as a string
        return "".join([b.Buffer for b in encbuf])

    def decrypt(self, data):
        """Decrypt a previously encrypted string, returning the orignal data"""
        pkg_size_info=self.ctxt.QueryContextAttributes(sspicon.SECPKG_ATTR_SIZES)
        trailersize=pkg_size_info['SecurityTrailer']
        msgsize = len(data)-trailersize
        assert msgsize >= 0, "trailer is %d bytes, but data only %d long" \
                             % (trailersize, len(data))

        encbuf=win32security.SecBufferDescType()
        encbuf.append(win32security.SecBufferType(msgsize, sspicon.SECBUFFER_DATA))
        encbuf.append(win32security.SecBufferType(trailersize, sspicon.SECBUFFER_TOKEN))
        encbuf[0].Buffer=data[:msgsize]
        encbuf[1].Buffer=data[msgsize:]
        self.ctxt.DecryptMessage(encbuf,1)
        return encbuf[0].Buffer

    def sign(self, data):
        """sign a string suitable for transmission.
        """
        pkg_size_info=self.ctxt.QueryContextAttributes(sspicon.SECPKG_ATTR_SIZES)
        sigsize=pkg_size_info['MaxSignature']
        sigbuf=win32security.SecBufferDescType()
        sigbuf.append(win32security.SecBufferType(len(data), sspicon.SECBUFFER_DATA))
        sigbuf.append(win32security.SecBufferType(sigsize, sspicon.SECBUFFER_TOKEN))
        sigbuf[0].Buffer=data

        self.ctxt.MakeSignature(0,sigbuf,1)
        # And return all buffers as a string
        return "".join([b.Buffer for b in sigbuf])

    def unsign(self, data):
        """Takes a message as a 'signed' string, verifies the signature and
        returns the original data"""
        pkg_size_info=self.ctxt.QueryContextAttributes(sspicon.SECPKG_ATTR_SIZES)
        sigsize=pkg_size_info['MaxSignature']
        msgsize = len(data)-sigsize
        assert msgsize >= 0, "signature is %d bytes, but data only %d long" \
                             % (sigsize, len(data))
        sigbuf=win32security.SecBufferDescType()
        sigbuf.append(win32security.SecBufferType(msgsize, sspicon.SECBUFFER_DATA))
        sigbuf.append(win32security.SecBufferType(sigsize, sspicon.SECBUFFER_TOKEN))

        sigbuf[0].Buffer=data[:msgsize]
        sigbuf[1].Buffer=data[msgsize:]
        self.ctxt.VerifySignature(sigbuf,1)
        return sigbuf[0].Buffer


class ClientAuth(_BaseAuth):
    """Manages the client side of an SSPI authentication handshake
    """
    def __init__(self,
                 pkg_name, # Name of the package to used.
                 client_name = None, # User for whom credentials are used.
                 auth_info = None, # or a tuple of (username, domain, password)
                 targetspn = None, # Target security context provider name.
                 scflags=None, # security context flags
                 datarep=sspicon.SECURITY_NETWORK_DREP):
        if scflags is None:
            scflags = sspicon.ISC_REQ_INTEGRITY|sspicon.ISC_REQ_SEQUENCE_DETECT|\
                      sspicon.ISC_REQ_REPLAY_DETECT|sspicon.ISC_REQ_CONFIDENTIALITY
        self.scflags=scflags
        self.datarep=datarep
        self.targetspn=targetspn
        self.pkg_info=win32security.QuerySecurityPackageInfo(pkg_name)
        self.credentials, \
        self.credentials_expiry=win32security.AcquireCredentialsHandle(
                client_name, self.pkg_info['Name'],
                sspicon.SECPKG_CRED_OUTBOUND,
                None, auth_info)
        _BaseAuth.__init__(self)

    # Perform *one* step of the client authentication process.
    def authorize(self, sec_buffer_in):
        if sec_buffer_in is not None and type(sec_buffer_in) != win32security.SecBufferDescType:
            # User passed us the raw data - wrap it into a SecBufferDesc
            sec_buffer_new=win32security.SecBufferDescType()
            tokenbuf=win32security.SecBufferType(self.pkg_info['MaxToken'],
                                                 sspicon.SECBUFFER_TOKEN)
            tokenbuf.Buffer=sec_buffer_in
            sec_buffer_new.append(tokenbuf)
            sec_buffer_in = sec_buffer_new
        sec_buffer_out=win32security.SecBufferDescType()
        tokenbuf=win32security.SecBufferType(self.pkg_info['MaxToken'], sspicon.SECBUFFER_TOKEN)
        sec_buffer_out.append(tokenbuf)
        ## input context handle should be NULL on first call
        ctxtin=self.ctxt
        if self.ctxt is None:
            self.ctxt=win32security.CtxtHandleType()
        err, attr, exp=win32security.InitializeSecurityContext(
            self.credentials,
            ctxtin,
            self.targetspn,
            self.scflags,
            self.datarep,
            sec_buffer_in,
            self.ctxt,
            sec_buffer_out)
        # Stash these away incase someone needs to know the state from the
        # final call.
        self.ctxt_attr = attr
        self.ctxt_expiry = exp
        
        if err in (sspicon.SEC_I_COMPLETE_NEEDED,sspicon.SEC_I_COMPLETE_AND_CONTINUE):
            self.ctxt.CompleteAuthToken(sec_buffer_out)
        self.authenticated = err == 0
        return err, sec_buffer_out

class ServerAuth(_BaseAuth):
    """Manages the server side of an SSPI authentication handshake
    """
    def __init__(self,
                 pkg_name,
                 spn = None,
                 scflags=None,
                 datarep=sspicon.SECURITY_NETWORK_DREP):
        self.spn=spn
        self.datarep=datarep
        
        if scflags is None:
            scflags = sspicon.ASC_REQ_INTEGRITY|sspicon.ASC_REQ_SEQUENCE_DETECT|\
                      sspicon.ASC_REQ_REPLAY_DETECT|sspicon.ASC_REQ_CONFIDENTIALITY
        # Should we default to sspicon.KerbAddExtraCredentialsMessage
        # if pkg_name=='Kerberos'?
        self.scflags=scflags

        self.pkg_info=win32security.QuerySecurityPackageInfo(pkg_name)

        self.credentials, \
        self.credentials_expiry=win32security.AcquireCredentialsHandle(spn,
                self.pkg_info['Name'], sspicon.SECPKG_CRED_INBOUND, None, None)
        _BaseAuth.__init__(self)

    # Perform *one* step of the server authentication process.
    def authorize(self, sec_buffer_in):
        if sec_buffer_in is not None and type(sec_buffer_in) != win32security.SecBufferDescType:
            # User passed us the raw data - wrap it into a SecBufferDesc
            sec_buffer_new=win32security.SecBufferDescType()
            tokenbuf=win32security.SecBufferType(self.pkg_info['MaxToken'],
                                                 sspicon.SECBUFFER_TOKEN)
            tokenbuf.Buffer=sec_buffer_in
            sec_buffer_new.append(tokenbuf)
            sec_buffer_in = sec_buffer_new

        sec_buffer_out=win32security.SecBufferDescType()
        tokenbuf=win32security.SecBufferType(self.pkg_info['MaxToken'], sspicon.SECBUFFER_TOKEN)
        sec_buffer_out.append(tokenbuf)
        ## input context handle is None initially, then handle returned from last call thereafter
        ctxtin=self.ctxt
        if self.ctxt is None:
            self.ctxt=win32security.CtxtHandleType()
        err, attr, exp = win32security.AcceptSecurityContext(self.credentials, ctxtin,
            sec_buffer_in, self.scflags,
            self.datarep, self.ctxt, sec_buffer_out)

        # Stash these away incase someone needs to know the state from the
        # final call.
        self.ctxt_attr = attr
        self.ctxt_expiry = exp

        if err in (sspicon.SEC_I_COMPLETE_NEEDED,sspicon.SEC_I_COMPLETE_AND_CONTINUE):
            self.ctxt.CompleteAuthToken(sec_buffer_out)
        self.authenticated = err == 0
        return err, sec_buffer_out

if __name__=='__main__':
   # Setup the 2 contexts.
    sspiclient=ClientAuth("NTLM")
    sspiserver=ServerAuth("NTLM")
    
    # Perform the authentication dance, each loop exchanging more information
    # on the way to completing authentication.
    sec_buffer=None
    while 1:
        err, sec_buffer = sspiclient.authorize(sec_buffer)
        err, sec_buffer = sspiserver.authorize(sec_buffer)
        if err==0:
            break
    assert sspiserver.unsign(sspiclient.sign("hello")) == "hello"
    assert sspiserver.decrypt(sspiclient.encrypt("hello")) == "hello"
    print "cool!"
 