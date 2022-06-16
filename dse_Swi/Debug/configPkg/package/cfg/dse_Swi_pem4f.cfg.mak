# invoke SourceDir generated makefile for dse_Swi.pem4f
dse_Swi.pem4f: .libraries,dse_Swi.pem4f
.libraries,dse_Swi.pem4f: package/cfg/dse_Swi_pem4f.xdl
	$(MAKE) -f C:\Users\mario.donjuan\workspace_v7\dse_Swi/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\mario.donjuan\workspace_v7\dse_Swi/src/makefile.libs clean

