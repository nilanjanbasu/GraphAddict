tk::canvas $nb.scrp.imageview -height 500 -width 950 -xscrollcommand "$nb.scrp.s_x set" -yscrollcommand "$nb.scrp.s_y set" -bg white -state normal -scrollregion { 0 0 10000 10000 }
ttk::scrollbar $nb.scrp.s_x -command "$nb.scrp.imageview xview" -orient h
ttk::scrollbar $nb.scrp.s_y -command "$nb.scrp.imageview yview" -orient v
grid $nb.scrp.imageview -in $nb.scrp -row 0 -column 0
grid $nb.scrp.s_x -in $nb.scrp -row 1 -column 0 -sticky ew
grid $nb.scrp.s_y -in $nb.scrp -row 0 -column 1 -sticky ns
set vizcanv $nb.scrp.imageview

menu .rmenub -tearoff false 
.rmenub add command -label "Save" -command ""


bind $vizcanv <3> {
	tk_popup .rmenub %X %Y
}

#proc SaveIt {} {
	#set tempfile ""
		#set tempfile [tk_getSaveFile]	
		#if {$tempfile!=""} {	
			#set ID [open $tempfile w]
			#save_image $tempfile
