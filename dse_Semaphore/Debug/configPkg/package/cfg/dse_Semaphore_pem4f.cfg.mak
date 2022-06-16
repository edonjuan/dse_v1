# invoke SourceDir generated makefile for dse_Semaphore.pem4f
dse_Semaphore.pem4f: .libraries,dse_Semaphore.pem4f
.libraries,dse_Semaphore.pem4f: package/cfg/dse_Semaphore_pem4f.xdl
	$(MAKE) -f C:\Users\mario.donjuan\workspace_v7\dse_Semaphore/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\mario.donjuan\workspace_v7\dse_Semaphore/src/makefile.libs clean

