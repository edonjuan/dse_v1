# invoke SourceDir generated makefile for dse_Hwi.pem4f
dse_Hwi.pem4f: .libraries,dse_Hwi.pem4f
.libraries,dse_Hwi.pem4f: package/cfg/dse_Hwi_pem4f.xdl
	$(MAKE) -f C:\Users\mario.donjuan\workspace_v7\dse_Hwi/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\mario.donjuan\workspace_v7\dse_Hwi/src/makefile.libs clean

