# invoke SourceDir generated makefile for empty.pem4f
empty.pem4f: .libraries,empty.pem4f
.libraries,empty.pem4f: package/cfg/empty_pem4f.xdl
	$(MAKE) -f C:\Users\mario.donjuan\workspace_v7\dse_Hwi/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\mario.donjuan\workspace_v7\dse_Hwi/src/makefile.libs clean

