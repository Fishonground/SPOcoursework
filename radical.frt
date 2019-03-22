
: is_prime 
	( validation for negative values )
	dup 0 < if ." Incorrect argument " else
	( somewhere I've made a mistake so 4 and 2 does not work - it's THE KOSTYL )	
	dup 2 = if drop 1 else
	dup 4 = if drop 0 else
	( it is by itself definition )	
	dup 2 < if drop 0 else
	2 >r 
			repeat  
				( save our variable value to stack )
				dup 
				( increment divider to receive next )
				r> 1 + dup >r  
				( check current divider not to be 0 )
				% 0 =  	
			until 
		r> = 
	then
	then
	then
	then
;

: radical
    dup 1 = not 
    ( check not to be equal 1 )
    if dup is_prime not 
        ( next step will work in case that value isn't prime )
        if dup >r 2 swap 
            ( ready to check for being even + saved value in stack )
            2 / swap r>
            ( received val/2 2 val in stack )
            1 >r
            ( val/2 2 val, 1 )
            repeat
                ( loop to receive value%i, where i is prime, without losing others vales in stack )
                >r dup r> swap >r dup r> swap swap %
		( block works in case val%i != 0 )
                if  >r dup r> swap >r rot dup r> >
                    ( check that the val/i > i for spot/continue dividing )
                    if rot 
			repeat
        		1 +
        		dup is_prime 
    			until 
			rot 0 
			( to continue main loop )
                    else 1 ( to stop loop of dividing )
                    then
                else
                    swap dup r> * >r 
			repeat
        		1 +
        		dup is_prime 
    			until 
                    ( to multipli all what we need and stop after )
                    swap 0
                    then
            until
            drop drop drop r> ( to put from stack result )
        then
    then 
;
