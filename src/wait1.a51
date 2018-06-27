NAME 	wait

EXTRN   DATA (Var1, Var2, Var3)

PUBLIC	w0ms, w1ms, w5mS, w50mS

Wait_ROUTINES  SEGMENT  CODE

	RSEG  Wait_ROUTINES

w0ms:                        ;waits for 2 processor cycles                    
        NOP
	NOP                  
	RET                  

w1ms:                         
        MOV	 Var2,#20     
        MOV	 Var1,#25      
TT2:	DJNZ Var1,TT2         
	     DJNZ Var2,TT2    
	     RET             

w5mS:                         
        MOV	 Var2,#12    
        MOV	 Var1,#200    
TT3:	DJNZ Var1,TT3   
	    DJNZ Var2,TT3    
	    RET  	        

w50mS: 
        MOV	 Var2,#115 
        MOV	 Var1,#200 
TT5:	DJNZ Var1,TT5   
	    DJNZ Var2,TT5   	
	    RET  	

END
