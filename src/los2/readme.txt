This program calculates the LOS files for Close Combat 2 maps.

usage:
		put the program in the root dir of CC2 (same folder as CC2.EXE)
		put your mapXXX in the data\maps folder
		start the programm
		type your map number
		drink a coffee (you've got the time)
		that's all

or command line:
		cclos [-T][+M mappath][+L lospath][+E basepath\elementfile][+N mapnumber]
			all path are with no file name
			-T : no text output
			if +M is defined and not +L, +M is used for output

In -T mode, the return value is:
			 0		all ok
			-1		bad map file
			-2		bad element file
			-3		not enought memory
			-4		map file not found
			-5		element file not found
			-6		can't create los file
			-7		disk full
			-8		bad element in map
			-9		invalid map number

I used (what i anderstand of) the methods given by JA, 
but the los generated is not equal to the atomic's LOS files. 
Neverless, the results don't seem too bad. 
(75% of information is the same)

For bug reports, you can contact me at: vonbister@hotmail.com
If you need help, first try the editing forum on
http://cc2.gamestats.com, please.

It's a dowhatwantware, so you can use it, modify it and use the
source code without even asking me. Just tell me if you think
it may be of interest for me.

Please if you make any changes that are useful post them to the
cc2.gamesstats.com editing forum.

Below is the information that give the 'pseudo' of LOS calculations.

--------

Unfortunately, the LOS tool is inherent in the game software 
and not a stand alone tool.  It requires manipulation through
the development environment to work.

You could probably come up with an LOS approximation utility 
using the following steps:


Load in the Elements file.
Load in the MapXXX.txt file.

For each look mega-tile (3x3 array of tiles) Do
    For each seen mega-tile Do
        For each tile in look mega-tile Do
            For each tile in seen mega-tile Do
                Calculate base elevation of the look terrain 
				  by taking the elevation of that tile's 5th element
                  (the .txt file is an index of tiles with 16 element
				  types and 16 elevations for each element in the tile)

				Add to this number 10 * the level of the element.
                  This accounts for multi-story buildings.  
				  The level is an entry in the Elements file near the 
				  end of the list (far right) for that element.

                Calculate the base elevation of the seen terrain as above.

                For every two meters distance from look point to seen
				  point Do 
				  
					Calculate the slope intercept (get the tile and
					  element at each point along the line)

	                Calculate the base elevation of this new point as
					  above, but add the terrain's object height.

	                Calculate the slope intercept's height from look
					  elevation + 2 to seen elevation +2

	                If slope intercept height < intercpet terrain's height
					Then	LOS is blocked
                
					Calculate the slope intercept's height from look
					  elevation + 2 to seen elevation +4
                  
					If slope intercept height < intercpet terrain's height
					Then	LOS is blocked
                     
					Calculate the slope intercept's height from look
					  elevation + 4 to seen elevation +2
                
					If slope intercept height < intercpet terrain's height
					Then	LOS is blocked
                     
					Calculate the slope intercept's height from look
					  elevation + 4 to seen elevation +4

	                If slope intercept height < intercpet terrain's height
					Then    LOS is blocked
	
	            Enddo

	            If number of +2 to +2 paths not blocked is 3 or more
					Set first LOS bit to 1 Else to 0
	            If number of +2 to +4 paths not blocked is 3 or more
	                Set second LOS bit to 1 Else to 0
	            If number of +4 to +2 paths not blocked is 3 or more
	                Set third LOS bit to 1 Else to 0
	            If number of +4 to +4 paths not blocked is 3 or more
	                Set fourth LOS bit to 1 Else to 0
			Enddo
		Enddo
    Enddo
Enddo


(method by JA
 send to me by:
 Gerry Shaw (aka TinTin)
 http://members.xoom.com/CloseCombat/)


