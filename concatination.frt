: concatinatt 
	( dup not to del str1 )
    dup 
	( count string lenght )
    count
	( now repeat with second srt )
    rot
    dup 
    count
	( now we have str2 len2 str1 len1 )
	( just replace )    
    rot 
    swap
	( now we have addr2 addr1 len2 len1 )
	( dup lenght to save value and use one time )
    dup 
    rot
	( add memory for concat string )
    + heap-alloc
	( replace and dup to receive futher values )
    rot 
    dup
    rot
    dup
	( now we have in stack str2 len1 str1 str1 str3 str3 )
	( del unusful values, recapture old memory )    
    rot 
    string-copy
    swap 
    heap-free
	( now we have in stack str2 len1 str3 )
	( now we should replace with dup several times to receive result string)    
    dup
    rot
    + 
    rot	
    dup 
    rot 
    swap
	( in stack str3 str2 str3-2 str2 )
	( last step - get str3 and recapture memory )
    string-copy 
    heap-free
	( gotovo!!! )
;
