( To check value for being even. It's incorrect , in my opinion, to create a new file for one string: )

: is_even 2 % ;


/////////////

( To check value for being prime:
0 for unprime, 1 for prime )

: is_prime 
	( validation for negative values )
	dup 0 < if ." Incorrect argument " else
	( somewhere I made a mistake so 4 and 2 does not work - it's THE KOSTYL )	
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

: is_prime-allot 
	( firstly we should receive result )
	is_prime
	( to allocate memory the same size as input )
	cell% allot
	( duplicate and it replace received result with adress )
	dup rot swap
	( write the result intu the memory )
	!
;


