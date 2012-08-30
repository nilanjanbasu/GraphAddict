#!/bin/sh
#Sreja Das Nilanjan Basu\
exec wish8.5 "$0" "$@"
load /usr/lib/tcltk/graphviz/tcl/libtcldot.so.0.0.0
load ./graph.so graph

wm title . "GraphAddictPro" 
image create photo temp -file "./bitmap/l.gif"
wm iconphoto . temp

#other global variables
set filename ""
set FileID ""
set head [createFlagPack]
set filenameSave ""
set fontsize 14
#end other global variables

#global variables depicting state of the opened file
set ifsaved 1
set GraphType ""
set CountOfFileNodes 0
set count 0
set countedge 0
set IfWeighted "Weighted"
set GVHandle [dotnew digraph rankdir LR]
set LayoutType "dot"
#end 
set ginfo "$filename $GraphType $IfWeighted" ;#needed for Display


#Declare that there is a menu
menu .mbar
. config -menu .mbar

#The Main Buttons
.mbar add cascade -label "File" -underline 0 \
      -menu [menu .mbar.file -tearoff 0]
.mbar add cascade -label "Edit" \
      -underline 1 -menu [menu .mbar.oth -tearoff 1]
.mbar add cascade -label "Help" -underline 0 \
      -menu [menu .mbar.help -tearoff 0]

## File Menu ##
set m .mbar.file
$m add command -label "New" -underline 0 \
	  -command "FileNewPressed" ;# A new item called New is added.
$m add command -label "Open" -underline 0 -command "OpenFile"
$m add command -label "Save" -underline 0 -command "SaveFile"
$m add command -label "Save As" -underline 1 -command "SaveAs"
$m add separator
$m add command -label "Exit" -underline 1 -command exit


#notebook for images
grid [ttk::frame .fr6 -padding "10 5"] -in . -row 0 -column 0 -columnspan 6
grid [ttk::notebook .fr6.draw ] -in .fr6 -row 0 -column 0 -sticky nwes
set nb .fr6.draw
ttk::frame $nb.drawhand -padding "10 5"
ttk::frame $nb.scrp -padding "10 5"
ttk::frame $nb.textbox -padding "10 5"
$nb add $nb.drawhand -text "Draw Graph"
$nb add $nb.textbox -text "Textbox"
$nb add $nb.scrp -text "Scrapbook"

source {scrapbook.tcl} ;#add scrapbook for viewing images
source {canvas.tcl} ;#add canvas 




proc OpenFile {} {	
		set ::filename [tk_getOpenFile]	
		if {$::filename==""} {
			return	
		}
		Load
		
	}
proc SaveFile {} {
		if {$::filename==""} {
			SaveAs
		} else {
			save_interface $::filename $::head
		}
	}	     
proc Load {} {	
		ClearAll 0;	
		set msg "Graph Loaded!!"
		if {$::filename==""} {
			return -1
		} else {
			if [catch {open $::filename r+} fp] {
				tk_messageBox -message "File can not be loaded\n(May be file does not exist?)" -type ok
				return 1
			} else {
				set ex [readInterface $::head $::filename]
				switch $ex {
					0 { set msg "Graph loaded successfully" }
					1 { set msg "File corrupt. Please enter a valid file" }
				}
				set tempw [return_weight $::head]
				if { $tempw==0 } {
					set ::IfWeighted "Weighted"
				}  else {
					set ::Ifweighted "Unweighted"
				}
				graphviz $::head "./tmp/_viz.dot"
				visualize "./tmp/_viz.dot" 1
				image create photo temp -file "./tmp/_temp_gr.gif"
				$::vizcanv create image 0 0 -anchor nw -image temp
				$::vizcanv configure -scrollregion [$::vizcanv bbox all]
								
				set fp [open $::filename r+]
				gets $fp lineone
				set st [string index $lineone end]
				set ::CountOfFileNodes [string trimright $lineone $st]
				close $fp
				
				set f [open "./tmp/_viz.dot" r+]
				set ::GVHandle [dotread $f]
				if {$st eq "u"} {					
					eval [$::GVHandle render $::f.board "dot"]
				} else {
					eval [$::GVHandle render $::f.board "dot"]
				}
				close $f
				file delete "./tmp/_viz.dot"
				$::GVHandle delete
				setGraphTypeFlag2 $st
				tk_messageBox -message $msg -type ok
				return 0
			}
		}
	}
proc setGraphTypeFlag2 {fl} {
		if {$fl=="d"} {
			set ::GraphType "Directed"
			setFlag2 $::head 0
			set ::LayoutType "dot"
		} else {
			set ::GraphType "Undirected"
			setFlag2 $::head 1
			set ::LayoutType "dot"
		}
}
proc FindPath {} {
		set msg ""
		if {$::f_node=="" || $::t_node==""} {
			set msg "Please enter valid source and/or destination node"
		} else {
			set err [ShortestPath $::f_node $::t_node $::head]
			switch err {
				1 { set msg "Error: No file loaded" }
				2 { set msg "Error: Graph was not loaded (head==NULL)" }
				3 { set msg "Error: No such SOURCE node" }
				4 { set msg "Error: No such DESTINATION node" }
				5 { set msg "Error: Source and Destination are same.(Trivial Case)" }
				6 { set msg "Error: Internal Memory Error"}
				7 { set msg "Sorry, no path found!!" }
				8 { set msg "Error: File error" }
			}
			if {$err !=0} {
				tk_messageBox -message $msg
			}
		}
	}

set st "u"
set wtt "Weighted"
proc FileNewPressed {} {
		if {$::ifsaved==0} {
			set ans [tk_messageBox -message "Save existing file before proceeding?" -type yesno -icon question]
			case $ans {
			 yes GetInputFile
			}
		}
		set ::ifsaved 0 ;# have to set it to correct value at proper places		
		set tempfile [tk_getSaveFile -defaultextension {.txt}]		
		if {$tempfile==""} {
			return -1
		} else {
			set ID [open $tempfile w]
			close $ID
			set ::filename $tempfile
		}	
		ClearAll 0;
		
		
		tk::toplevel .top2 -bg #F0E68C
		wm resizable .top2 0 0
		wm title .top2 "Configuration Window"
		set in [winfo parent .top2]
		set y [winfo screenheight $in]
		set x [winfo screenwidth $in]
		wm geometry .top2 260x100+[expr {$x/2 -75} ]+[expr {$y/2} ]
		grid [ttk::label .top2.l -text "Graph type:"] -row 1 -column 1 -sticky ew
		grid [ttk::radiobutton .top2.dir -text {Directed} -variable ::st -value d] -row 1 -column 2 
		grid [ttk::radiobutton .top2.un -text {Undirected} -variable ::st -value u] -row 1 -column 3
		grid [ttk::radiobutton .top2.wgt -text {Weighted} -variable ::wtt -value "Weighted"] -row 2 -column 2
		grid [ttk::radiobutton .top2.unwgt -text {Unweighted} -variable ::wtt -value "Unweighted"] -row 2 -column 3
		
		grid [ttk::button .top2.bt -text "Ok" -command "destroy .top2; PressedOkay"] -row 3 -column 2 -sticky ew
		
}
proc PressedOkay {} {
		puts "graph type-> $::st"; 
		setGraphTypeFlag2 $::st
		if {$::wtt eq "Weighted"} {
			setstatus 0 $::head
			puts "xx"
		} else {
			setstatus 1 $::head
			puts "yy"
		}		
}
proc ClearAll {iffile} {
		init_pass $::head		
		$::f.board delete all
		if { [info exists GVHandle] } { $GVHandle delete }
		set ::CountOfFileNodes 0
		
		set ::ifsaved 1
		set ::GraphType ""
		set ::CountOfFileNodes 0
		set ::count 0
		set ::countedge 0
		set ::IfWeighted "Weighted"		
}
		
proc SaveAs {} {
		set tempfile ""
		set tempfile [tk_getSaveFile]	
		if {$tempfile!=""} {
			save_interface $tempfile $::head ;#New version of save implemented
		} 
}	

###Please verify the code below
proc GetInputFile {} {
					tk::toplevel .top2 -bg #F0E68C
					wm resizable .top2 0 0
					wm title .top2 "File input"
					set in [winfo parent .top2]
					set y [winfo screenheight $in]
					set x [winfo screenwidth $in]
		
					wm geometry .top2 240x100+[expr {$x/2 -75} ]+[expr {$y/2} ]
					grid [ttk::label .top2.l -text "Enter Filename:"] -row 1 -column 1 -sticky ew
					grid [tk::entry .top2.ent -textvariable ::filenameSave -width 15] -row 1 -column 2 
					grid [ttk::frame .top2.sp1 -height 20] -row 0 -column 1 -columnspan 2
					grid [ttk::frame .top2.sp2 -height 20] -row 2 -column 1 -columnspan 2
					grid [ttk::frame .top2.sp3 -width 15] -row 0 -column 0 -rowspan 2
					grid [ttk::frame .top2.sp4 -width 15] -row 0 -column 3 -rowspan 2
			
					grid [ttk::button .top2.bt -text "Ok" -command "SaveIt ; return $::filenameSave"] -row 3 -column 1
					grid [ttk::button .top2.bt2 -text "Cancel" -command "destroy .top1"] -row 3 -column 2 -sticky e
		
					focus .top2.ent
}
proc  SaveIt {} {
			destroy .top2
			save_interface $::filenameSave $::head 
		}
