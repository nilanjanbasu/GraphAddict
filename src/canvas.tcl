
set f $nb.drawhand

#Drawing box
grid [ttk::frame $f.opt -padding "7 12" -relief "raised"] -in $f -row 0 -column 0 -rowspan 3 -sticky ns
grid [ttk::label $f.opt.l -text {Your options:}] -in $f.opt -row 0 -column 0 -sticky w

grid [ttk::button $f.opt.b1 -text {Add Node} -command "set modeofOperation 1" ] -in $f.opt -row 1 -column 0
grid [ttk::button $f.opt.b2 -text {Add Edge} -command "set modeofOperation 2" ] -in $f.opt -row 2 -column 0
#grid [ttk::button $f.opt.b3 -text {Done} -command "GetInputFile " ] -in $f.opt -row 3 -column 0
grid [ttk::checkbutton $f.opt.check -text "Weight\nWindow" -variable ::IfWeighted \
			-onvalue "Weighted" -offvalue "Unweighted"] -in $f.opt -row 3 -column 0 -sticky w
grid [ttk::button $f.opt.b4 -text {View Mode} -command "viewMode"] -in $f.opt -row 4 -column 0
#grid [ttk::button $f.opt.b5 -text {Clear} -command "clear" ] -in $f.opt -row 5 -column 0
grid [ttk::button $f.opt.b6  -text {Refresh} -command "reload" ] -in $f.opt -row 5 -column 0

grid [ttk::label $f.lb -textvariable ginfo ] -row 0 -column 1 -sticky ew		
grid [tk::canvas $f.board -height 500 -width 850] -in $f -row 1 -column 1 -rowspan 2
grid [ttk::scrollbar $f.s_x -command "$f.board xview" -orient h] -in $f -row 3 -column 1 -sticky ew 
grid [ttk::scrollbar $f.s_y -command "$f.board yview" -orient v] -in $f -row 1 -column 2 -sticky ns -rowspan 2

$f.board config -scrollregion { 0 0 10000 10000 } -xscrollcommand "$f.s_x set" -yscrollcommand "$f.s_y set" -bg white \
		-state normal

ttk::entry $f.board.ent -width 5 -textvariable labelname ; #entry for planting in canvas to rename the nodes
set txtent $f.board.ent

set alochyo ""
set lastVisited ""
set modeofOperation 0	
#bind $f.board <3> "zoomMark $f.board %x %y"
#bind $f.board <B3-Motion> "zoomStroke $f.board %x %y"
#bind $f.board <ButtonRelease-3> "zoomArea $f.board %x %y"

bind $f.board  <Shift-Button-1> "Mark %x %y"
bind $f.board <Shift-B1-Motion> "Move %x %y"
bind $f.board <Shift-ButtonRelease-1> {
	set ::selected ""
	set ::selected2 ""
}	
bind $f.board <1> {
	switch $modeofOperation {
	1 { GetNodeNameWindow %x %y; }
	2 { DrawEdge %x %y;}
	}
}
###############################################################################
bind $f.board <Double-3> { coolkaal $::lastVisited }
proc coolkaal {l} {
	puts $l
}

#####################################################################################

proc globalEventBindings {} {

#bind $f.board <3> "zoomMark $f.board %x %y"
#bind $f.board <B3-Motion> "zoomStroke $f.board %x %y"
#bind $f.board <ButtonRelease-3> "zoomArea $f.board %x %y"

bind $::f.board  <Shift-Button-1> "Mark %x %y"
bind $::f.board <Shift-B1-Motion> "Move %x %y"
bind $::f.board <Shift-ButtonRelease-1> {
	set ::selected ""
	set ::selected2 ""
} 
bind $f.board <1> {
	switch $modeofOperation {
	1 { GetNodeNameWindow %x %y; }
	2 { DrawEdge %x %y;}
	}
   }
}
$f.board bind all <Enter> { 
		puts enter
		     if {[lsearch [$::f.board gettags current] ent0]!=-1} { puts yoo; break; }
		     set alltags [$::f.board gettags current]
		     set tempindx [lsearch $alltags current]
		     set ::alochyo [lreplace $alltags $tempindx $tempindx]
		     set tempindx [lsearch $::alochyo text]
		     set ::alochyo [lreplace $::alochyo $tempindx $tempindx]
		     if {[string range $::alochyo 0 0]!=0} {
		     	     $::f.board itemconfigure $::alochyo -width 3;	      
		     }
}
$f.board bind all <Leave> {
puts leave 
			#if {[string match ent0 $::alochyo]==1} { break; } ;#so that lastVisited is not modified
			if {[string range $::alochyo 0 0]!=0} {
				$::f.board itemconfigure $::alochyo -width 1;
			}
			set ::lastVisited $::alochyo		
			set ::alochyo ""
}

menu .rmenu0n -tearoff false 
menu .rmenu1n -tearoff false
menu .rmenu0e -tearoff false
menu .rmenu1e -tearoff false
menu .rmenublank -tearoff false

.rmenu0n add command -label "Rename" -command "rename"

.rmenu1n add command -label "Delete" -command "deleteNodeFromCanvas"
.rmenu1n add command -label "Find Clustering Coefficient" -command "Clustering"

.rmenu0e add command -label "Rename" -command ""
.rmenu1n add command -label "Shortest Path" -command "Getnodename"
.rmenu1e add command -label "Delete" -command "delEdgeFromCanvas"
.rmenu1n add command -label "Properties" -command "info1"
.rmenublank add command -label "Degree Distribution" -command "GnuPlotCanvas"
.rmenublank add command -label "properties" -command ""

bind $f.board <3> {
	if {[string match 0node* $::alochyo]} { #for a text item
		tk_popup .rmenu0n %X %Y
	} elseif {[string match 1node* $::alochyo]} {
		tk_popup .rmenu1n %X %Y
	} elseif {[string match 0edge* $::alochyo]} {
	} elseif {[string match 1edge* $::alochyo]} {
		tk_popup .rmenu1e %X %Y
	} else {
		set a [getHead $::head]
		if { "$a" ne "NULL" } { 
			tk_popup .rmenublank %X %Y 
		}		
	}
}
proc rename {} {
	set tgname $::lastVisited
	if {[info exists oldval]==1} {
		$::f.board delete $::txtent
		global oldval;global oldtg;
		$::f.board create text $coords -text $oldval -tags "$oldtg" -fill black -font {"Times Roman" 14}
	}
	global oldval;
	global oldtg;
	if {[string match 0node* $tgname]==1} { #ie it is a nodelabel
		set oldval [createEntry $tgname]
		set oldtg $tgname		
	} 
}
bind $txtent <Return> {
	
	set coords [$::f.board coords ent0]
	global oldtg
	if {[llength $coords]==2} {
		if {$::labelname==""} { global oldval;set ::labelname $oldval;}
		$::f.board create text $coords -text $::labelname -tags "$oldtg" -fill black -font [list "Times Roman" $::fontsize ]
		$::f.board delete ent0		
		set vhead [getHead $::head]		
		changeName $::labelname [locatev $oldval $vhead]
		unset oldval;
		unset oldtg;
	}	
	
}
proc createEntry {tg} {
	set ::labelname [$::f.board itemcget $tg -text]
	set coords [$::f.board coords $tg]
	set oldval $::labelname
	$::f.board delete $tg
	$::f.board create window $coords -window $::txtent -tags ent0
	$::txtent select range 0 end
	focus $::txtent
	return $oldval
}
proc delEdgeFromCanvas {} {	
	deleteLocalEdges $::lastVisited
} 
proc findLineAdjNode {t mode} {
	set select ""
	set wt [$::f.board find withtag $t]
	if {[llength $wt] != 1} { #select the line item from line and polygon item
		 foreach a $wt {
		 	if { [$::f.board type $a] == "line" } {
		 		set select $a
		 	}
		 }
	} else {
		set select $t
	}	
	set cds [$::f.board coords $select]
	set selCds [list]
	
	if {$mode == "start"} {	
		set selCds [lrange $cds 0 1]
	}
	if {$mode== "end"} {
		set selCds [lrange $cds end-1 end]
	} 
	if {$mode=="both"} {
		set selCds [lrange $cds 0 1]
		lappend selCds [lrange $cds end-1 end]
	}
	
	set nodeTags [list]
	foreach {i j} $selCds {
		set ovrlp [$::f.board find overlapping [expr {$i-2}] [expr {$j-2}] [expr {$i+2}] [expr {$j+2}] ]							  
		foreach yyy $ovrlp {
				set t [$::f.board gettags $yyy]
				if { [string match 1node* $t] } {
					lappend nodeTags $t
					break;
					}
		}
	}	
	return $nodeTags;
}
proc reload {} {
	$::f.board delete all
	set ::countedge 0
	#if {[file exists "./tmp/_viz.dot"]==1} {
		#file delete "./tmp/_viz.dot"
	#}
	graphviz $::head "./tmp/_viz.dot"
	visualize "./tmp/_viz.dot" 0
	set f [open "./tmp/_viz.dot" r+]
	set ::GVHandle [dotread $f]
	eval [$::GVHandle render $::f.board $::LayoutType]
	set ::count 0
	close $f
	$::GVHandle delete
}
proc deleteNodeFromCanvas {} { 
	set operate $::lastVisited
	$::f.board dtag current ; #needed because it was being included with taglist of the node to be deleted in nodedeletion
	set ldone [list]; #list that records the edges that had been operated on
	if {[ string match *node* $operate ]==1} {;#i.e. it is a node(text or oval)
		set matching [$::f.board coords "1node[string range $operate 5 end]"]
		set matching [getbox $matching]
		set ngbrs [$::f.board find overlapping [lindex $matching 0] [lindex $matching 1] [lindex $matching 2] [lindex $matching 3]]
		foreach fig $ngbrs { #foreach neighbor edge and labels ids
		puts "ngbrs: $fig"
		puts "tags: [set t [$::f.board gettags $fig]]" ;#get the tag of those edges
			if {[string match *edge* $t] == 1} { # if it is edge
				if {[lsearch $ldone $t]==-1} {
					deleteLocalEdges $t
					set ldone [linsert $ldone 0 $t]
				}
			}
		}
		set derName [$::f.board itemcget "0node[string range $operate 5 end]" -text]
		
		$::f.board delete "1node[string range $operate 5 end]"
		$::f.board delete "0node[string range $operate 5 end]"
		deleteNode $::head $derName
	}
}
proc getbox {cds} { #give the coords list to get a rectangle surrounding the item
	lset cds 0 [expr {[lindex $cds 0]-2}]
	lset cds 1 [expr {[lindex $cds 1]-2}]
	lset cds 2 [expr {[lindex $cds 2]+2}]
	lset cds 3 [expr {[lindex $cds 3]+2}]
	return $cds
}
proc deleteLocalEdges {EtoDel} {

	set Edgetgs ""
	set no [$::f.board find withtag $EtoDel]	
	
	if {[string range $EtoDel 5 5]!="*" && $::GraphType eq "Directed"} { #it was added manually by drawing
		foreach item $no {
			switch -exact [$::f.board type $item] {
				"polygon" { 
						set box [$::f.board bbox $item]
						set ovrlp [$::f.board find overlapping [lindex $box 0] [lindex $box 1] [lindex $box 2] [lindex $box 3]]
						foreach yyy $ovrlp {
							set t [$::f.board gettags $yyy]
							if { [string match 1node* $t] } {
								set Edgetgs $t
								break;
							}
						}
					   }
			}
		}
		puts "Overlapping1: $Edgetgs"
	} else {
		set Edgetgs [findLineAdjNode $EtoDel end]
		puts "Overlapping1: $Edgetgs"
	}
	set ptrlst [list]
	set Edgetgs [linsert $Edgetgs 0 [findLineAdjNode $EtoDel start]	] ;#problem line
	foreach et $Edgetgs {
		puts "edgetags: $et"
		lappend ptrlst [$::f.board itemcget "0[string range $et 1 end]" -text]
	}
	puts "[lindex $ptrlst 0]to[lindex $ptrlst 1]"
	deleteEdgeDirect [lindex $ptrlst 0] [lindex $ptrlst 1] $::head
	if {$::GraphType eq "Undirected"} {
		deleteEdgeDirect [lindex $ptrlst 1] [lindex $ptrlst 0] $::head
	}
	$::f.board delete $EtoDel
	$::f.board delete "0[string range $EtoDel 1 end]"
	incr ::countedge -1	
} 
proc GnuPlotCanvas {} {
	gnuplot $::head;
	image create photo temp -file "./tmp/image_12345.gif"
	$::vizcanv create image 0 0 -anchor nw -image temp
}
proc viewMode {} {
	set ::zoomfont $::fontsize
	tk::toplevel .vmode -bg white 
	wm title .vmode "View Mode"
	#wm geometry .vmode 1400x1400+0+0
	menu .vmode.menu
	.vmode config -menu .vmode.menu
	.vmode.menu add command -label "Zoom In" -underline 0 -command "zoom .vmode.c 1.2"
	.vmode.menu add command -label "Zoom Out" -underline 0 -command "zoom .vmode.c 0.8"
	.vmode.menu add command -label "Font +" -command { incr ::zoomfont ; .vmode.c itemconfigure "text" -font [list "Times Roman" $::zoomfont ]}
	.vmode.menu add command -label "Font -" -command { incr ::zoomfont -1 ; .vmode.c itemconfigure "text" -font [list "Times Roman" $::zoomfont ]}
	
	grid [tk::canvas .vmode.c -bg white -xscrollcommand ".vmode.x set" -yscrollcommand ".vmode.y set" -height 600 -width 900 ] -in .vmode -row 0 -column 0 
	grid [ttk::scrollbar .vmode.x -command ".vmode.c xview" -orient h] -in .vmode -row 1 -column 0 -sticky ew
	grid [ttk::scrollbar .vmode.y -command ".vmode.c yview" -orient v] -in .vmode -row 0 -column 1 -sticky ns
	graphviz $::head "./tmp/_viz.dot"
	set f [open "./tmp/_viz.dot" r+]
	set handle [dotread $f]
	eval [$handle render .vmode.c $::LayoutType]
	close $f
	file delete "./tmp/_viz.dot"
	eval [$handle delete]
	set border [.vmode.c bbox all]
	.vmode.c config -scrollregion $border
	
	foreach item [.vmode.c find withtag all] {
		switch -exact [.vmode.c type $item] {
                	"text" { .vmode.c addtag text withtag $item }
               }
        }   
}
	
	
#######################################################################################################################

proc GetNodeNameWindow {h v} {

		tk::toplevel .top -bg #F0E68C
		wm resizable .top 0 0
		wm title .top "Node name input"
		set in [winfo parent .top]
		set y [winfo screenheight $in]
		set x [winfo screenwidth $in]
		
		wm geometry .top 240x100+[expr {$x/2 -75} ]+[expr {$y/2} ]
		grid [ttk::label .top.l -text "Enter Name:"] -row 1 -column 1 -sticky ew
		grid [tk::entry .top.ent -textvariable ::name -width 15] -row 1 -column 2 
		grid [ttk::frame .top.sp1 -height 20] -row 0 -column 1 -columnspan 2
		grid [ttk::frame .top.sp2 -height 20] -row 2 -column 1 -columnspan 2
		grid [ttk::frame .top.sp3 -width 15] -row 0 -column 0 -rowspan 2
		grid [ttk::frame .top.sp4 -width 15] -row 0 -column 3 -rowspan 2
		puts $::name
		grid [ttk::button .top.bt -text "Ok" -command "destroy .top ; DrawNode $h $v"] -row 3 -column 1
		grid [ttk::button .top.bt2 -text "Cancel" -command "destroy .top"] -row 3 -column 2 -sticky e
		
		focus .top.ent
}
		
proc DrawNode {h v} {
		puts "$h $v"
		set len [string length $::name]
		addOval $h $v $len	
}
proc addOval {mx my len} {  #FutureWork: Consider names greater than 15 in length:include in major axis of ellipse
			set lx [$::f.board canvasx $mx]
			set y [$::f.board canvasy $my]
			puts "$lx $y"
			set firstx [expr {$lx-($len)/2-50}]			
			set firsty [expr {$y-25.455}]
			set lastx [expr {$lx+($len)/2+50}]			
			set lasty [expr {$y+25.455}]
			set x1 $lx
			set t $::count
			set id [$::f.board create oval $firstx $firsty $lastx $lasty -tag "1node*$t" -fill white -width 1 -outline black]
			$::f.board create text $x1 $y -text $::name -tag "0node*$t" -font [list "Times Roman" $::fontsize ]
			addVertex $::head $::name
			incr ::count	
}
set sonkhya 0
set coord1 [list]
set coord2 [list]
set PassableTags [list]
set weight 1
#proc AddEdge {} {
#			bind  $::f.board <Button-1> "DrawEdge %x %y"	
#		}
proc DrawEdge {mx my} {
			set result [list]
			set ::weight 1
			set x [$::f.board canvasx $mx]
			set y [$::f.board canvasy $my]
			
			
			if {$::sonkhya==0} {
				set ::PassableTags [list]; #reinitialize list
				set ::coord1 [list]
				set ::coord2 [list]
				set ::weight 1
			}
			set tg $::alochyo
			if {$tg==""} {
				tk_messageBox -message "Somthing went wrong! Please try again!" -icon info -title "Sorry!"
				return
			}
			
			if {[string match *node* $tg]==1} { 
				if {[string range $tg 0 0]==0} {					
					set xx [string range $tg 1 6]
					set yy "1$xx"
					set xx [$::f.board coords $yy]
					set result [getOvalCenterMA $xx]
				} else {
					set result [getOvalCenterMA [$::f.board coords $tg]]
				}
				set ::PassableTags [linsert $::PassableTags 0 $tg] ;#inserted in order: to,from
				puts "Mytags => $tg"
				if {$::sonkhya==0} {
					set ::coord1 $result
					set ::sonkhya 1
				} else {
					set ::coord2 $result
					incr ::sonkhya
				}
			}						

			if {$::sonkhya==2 } {
				puts "$::coord1 $::coord2"
				Edge $::coord1 $::coord2				
				if { $::IfWeighted=="Weighted"} {
					tk::toplevel .top1 -bg #F0E68C
					wm resizable .top1 0 0
					wm title .top1 "weight input"
					set in [winfo parent .top1]
					set y [winfo screenheight $in]
					set x [winfo screenwidth $in]
					wm geometry .top1 240x100+[expr {$x/2 -75} ]+[expr {$y/2} ]
					grid [ttk::label .top1.l -text "Enter weight:"] -row 1 -column 1 -sticky ew
					grid [tk::entry .top1.ent -textvariable ::weight -width 15] -row 1 -column 2 ;#may cause problem
					grid [ttk::frame .top1.sp1 -height 20] -row 0 -column 1 -columnspan 2
					grid [ttk::frame .top1.sp2 -height 20] -row 2 -column 1 -columnspan 2
					grid [ttk::frame .top1.sp3 -width 15] -row 0 -column 0 -rowspan 2
					grid [ttk::frame .top1.sp4 -width 15] -row 0 -column 3 -rowspan 2
			
					grid [ttk::button .top1.bt -text "Ok" -command "DrawWeightText ; destroy .top1"] -row 3 -column 1
					grid [ttk::button .top1.bt2 -text "Cancel" -command "destroy .top1"] -row 3 -column 2 -sticky e
					focus .top1.ent
					
				}			
				AllocateMemoryEdge $::PassableTags $::weight
				set ::sonkhya 0	
				incr ::countedge		
			}		
}
proc DrawWeightText {} { 

		set x [lindex $::coord1 0]
		set y [lindex $::coord1 1]
		set a [lindex $::coord2 0]
		set b [lindex $::coord2 1]
		set m [expr {$x+$a}]
		set m [expr {$m/2+5}] ;#extra 5pixels added to avoid superposition
		set n [expr {$y+$b}]
		set n [expr {$n/2-5}]
		
		set x [expr {$::countedge-1}]
		$::f.board create text $m $n -text $::weight -tag "0edge*$x"
}
proc getOvalCenterMA {c} {
				set x [expr {[lindex $c 0]+[lindex $c 2]}]
				set mj [expr {[lindex $c 2]-[lindex $c 0]}]
				set mj [expr {$mj/2}]
				set x [expr {$x/2}]
				set y [expr {[lindex $c 1]+[lindex $c 3]}]
				set y [expr {$y/2}]			;#minor axis info can be included here
				
				return [list $x $y $mj]
}
proc AllocateMemoryEdge {tags wt} {
				set vertexptrlist [list]	
				foreach tag $tags { 					
					if {[expr [string range $tag 0 0]]==1} { # ie selected item is an oval
					     set t "0node[string range $tag 5 end]"							
					} else { #ie selected item is text
						set t $tag
					}
					puts [set txt [$::f.board itemcget $t -text]]
					set x [locatev $txt [getHead $::head]]
					set vertexptrlist [linsert $vertexptrlist 0 $x]					
				}
				
				if {$::IfWeighted=="Unweighted"} {
					addEdge $::head [lindex $vertexptrlist 0] [lindex $vertexptrlist 1] 1 $::GraphType
				} else {
					if {$wt==""} {
						set wt 1
					}
					addEdge $::head [lindex $vertexptrlist 0] [lindex $vertexptrlist 1] $wt $::GraphType
				}
}

proc Edge {c1 c2} {
			set x [lindex $c1 0]
			set y [lindex $c1 1]
			set a [lindex $c2 0]
			set b [lindex $c2 1]
			set mj1 [lindex $c1 2]
			set mj2 [lindex $c2 2]
			set cList [getList $x $y $a $b $mj1] ;#minor axis is taken to be 25.455 which is GV standard
			set dList [getList $a $b $x $y $mj2]
			puts "Grapgtype: $::GraphType"
			if {$::GraphType eq "Directed"} {
	 			set cmd "last"
	 		} else {
	 			set cmd "none"
	 		}
	 		set lineid [$::f.board create line [concat $cList $dList] -smooth bezier -tag "1edge*$::countedge" -arrow $cmd]
	 		puts "Edge created: 1edge*$::countedge"
	 			 		
}	
proc getList {x y a b mj} { 
		set den [expr {[expr {$x-$a}]*25.455} ]
		set num [expr {[expr {$y-$b}]*$mj} ]
		set theta [::tcl::mathfunc::atan [expr {$num/$den}] ]
		set ptx [expr { $mj*[::tcl::mathfunc::cos $theta]+$x }]
 		set pty [expr { 25.455*[::tcl::mathfunc::sin $theta]+$y }]
 		set l1 [length $ptx $pty $a $b]
		set theta [expr {$theta+3.1415926535897931}]
 		set ptx2 [expr { $mj*[::tcl::mathfunc::cos $theta]+$x }]
 		set pty2 [expr { 25.455*[::tcl::mathfunc::sin $theta]+$y }]
 		set l2 [length $ptx2 $pty2 $a $b]
 		
 		if {$l2>$l1} {
 			set coord [list $ptx $pty]
 		} else {
 			set coord [list $ptx2 $pty2]
 		}
 		return $coord
 	}
proc length {x y a b} {
		set xdist [expr {$x-$a}]
		set ydist [expr {$y-$b}]		
		set x [expr [expr {$xdist*$xdist+$ydist*$ydist} ]]
		set ans [expr [::tcl::mathfunc::sqrt $x] ]
		return $ans
}


proc zoomMark {c x y} {
        global zoomArea
        set zoomArea(x0) [$c canvasx $x]
        set zoomArea(y0) [$c canvasy $y]
        $c create rectangle $x $y $x $y -outline black -tag zoomArea
    }


proc zoomStroke {c x y} {
        global zoomArea
        set zoomArea(x1) [$c canvasx $x]
        set zoomArea(y1) [$c canvasy $y]
        $c coords zoomArea $zoomArea(x0) $zoomArea(y0) $zoomArea(x1) $zoomArea(y1)
    }


proc zoomArea {c x y} {
        global zoomArea
       set zoomArea(x1) [$c canvasx $x]
	#check here
        set zoomArea(y1) [$c canvasy $y]
        $c delete zoomArea
        if {($zoomArea(x0)==$zoomArea(x1)) || ($zoomArea(y0)==$zoomArea(y1))} {
            return
        }
        set areaxlength [expr {abs($zoomArea(x1)-$zoomArea(x0))}]
        set areaylength [expr {abs($zoomArea(y1)-$zoomArea(y0))}]
        set xcenter [expr {($zoomArea(x0)+$zoomArea(x1))/2.0}]
        set ycenter [expr {($zoomArea(y0)+$zoomArea(y1))/2.0}]
        set winxlength [winfo width $c]
        set winylength [winfo height $c]
        set xscale [expr {$winxlength/$areaxlength}]
        set yscale [expr {$winylength/$areaylength}]
        if { $xscale > $yscale } {			
            set factor $yscale;#why choose smaller
        } else {
            set factor $xscale
        }
        zoom $c $factor $xcenter $ycenter $winxlength $winylength
    }
set i 0
set j 0
set zoomfont 1
proc zoom { canvas factor \
            {xcenter ""} {ycenter ""} \
            {winxlength ""} {winylength ""} } {
        set winxlength [winfo width $canvas]; # Always calculate [ljl]
        set winylength [winfo height $canvas]
        if { [string equal $xcenter ""] } {
            set xcenter [$canvas canvasx [expr {$winxlength/2.0}]];#why use canvasx
            set ycenter [$canvas canvasy [expr {$winylength/2.0}]]
        }
        $canvas scale all 0 0 $factor $factor
        set xcenter [expr {$xcenter * $factor}]
        set ycenter [expr {$ycenter * $factor}]
        set x0 1.0e30; set x1 -1.0e30 ;
        set y0 1.0e30; set y1 -1.0e30 ;
        foreach item [$canvas find all] {
            switch -exact [$canvas type $item] {
                "arc" -
                "line" -
                "oval" -
                "polygon" -
                "rectangle" {
                    set coords [$canvas coords $item]
                    foreach {x y} $coords {
                        if { $x < $x0 } {set x0 $x}
                        if { $x > $x1 } {set x1 $x}
                        if { $y < $y0 } {set y0 $y}
                        if { $y > $y0 } {set y1 $y}
                    }
                }
            }
        }
        set xlength [expr {$x1-$x0}]
        set ylength [expr {$y1-$y0}]
	set f 1
	set flag 0
	
        foreach {ax0 ay0 ax1 ay1} [$canvas bbox all] {break}

        while { ($ax0<$x0) || ($ay0<$y0) || ($ax1>$x1) || ($ay1>$y1) } {
            # triple the scalable area size
            set x0 [expr {$x0-$xlength}]
            set x1 [expr {$x1+$xlength}]
            set y0 [expr {$y0-$ylength}]
            set y1 [expr {$y1+$ylength}]
            set xlength [expr {$xlength*3.0}]
            set ylength [expr {$ylength*3.0}]
        }
        set newxleft [expr {($xcenter-$x0-($winxlength/2.0))/$xlength}]
        set newytop  [expr {($ycenter-$y0-($winylength/2.0))/$ylength}]
        $canvas configure -scrollregion [list $x0 $y0 $x1 $y1]
        $canvas xview moveto $newxleft
        $canvas yview moveto $newytop
	identifyText
	
	set ::zoomfont [ expr { int($::zoomfont*$factor)}]
	if { $::zoomfont < 4 && $factor <1 } {
		set ::zoomfont 4
		set flag 1
		incr ::i
	}
	if { $::j<$::i && $factor >1} {
		set ::zoomfont 6
		incr ::j
	}
	if {$::j==$::i && $factor >1 } {
		set flag 0
		set ::j 0
		set ::i 0
	}
	$canvas itemconfigure "text" -font [list "Times Roman" $::zoomfont ]
        $canvas configure -scrollregion [$canvas bbox all]
    }

set select [list]
set atx 0
set aty 0
set selected ""
set selected2 ""
proc Mark {mx my} {
		if {$::alochyo!=""} {
			set v [ string match "*node*" $::alochyo]		
			if { $v!=0 } {
				set ::selected $::alochyo
				set p [ string range $::alochyo 0 0]
				if { $p==0 } {
					set ::selected2 "1node[ string range $::alochyo 5 end]"
				} else {
					set ::selected2 "0node[ string range $::alochyo 5 end]"
				}
			}
			set ::atx [ $::f.board canvasx $mx]
			set ::aty [ $::f.board canvasy $my]
	}
}
proc Move { mx my } {
	set x [ $::f.board canvasx $mx]
	set y [ $::f.board canvasy $my ]
	set changed_x [expr { $x - $::atx }]
	set changed_y [expr { $y - $::aty }]
	$::f.board move $::selected $changed_x $changed_y
	$::f.board move $::selected2 $changed_x $changed_y
	set ::atx $x
	set ::aty $y
	}

proc clear {} {
	$::f.board delete all
	}

proc identifyText {} {
		set itemlist [$::f.board find all]
		foreach num $itemlist {
				set tag [$::f.board gettag $num]
				set p [string match "0node*" $tag ]
				if { $p==1 } {
					$::f.board addtag "text" withtag $tag
				}
			}
}
#The code that makes objects bold when mouse pointer enters them

proc Clustering {} {
	    set tgname $::lastVisited
		if {[string match 0node* $tgname]==1} {
		set label[$::f.board itemcget $tgname -text]
	} elseif {[string match 1node* $tgname]==1} {
		set xx [string range $tgname 1 end]
		set label "0$xx"
	}
	puts $label
	set label [$::f.board itemcget $label -text]
	puts $label
	set ans [cluster_interface $::head $label]
	if {$ans>=0} {
	tk_messageBox -message "The required clustering coefficient is $ans" -icon info -title Answer
	} else {
			if {$ans==-1} {
				set D 0
			} else {
			set D 1
			}
		tk_messageBox -message "Clustering coefficient can not be found as G(v)=$D" -icon warning -title Answer
 }
}



set destination ""
proc Getnodename {} {
	 set tgname $::lastVisited
	puts $tgname
	tk::toplevel .top5 -bg #F0E68C
					wm resizable .top5 0 0
					wm title .top5 "Destination node input"
					set in [winfo parent .top5]
					set y [winfo screenheight $in]
					set x [winfo screenwidth $in]
					#set text ""
					wm geometry .top5 240x100+[expr {$x/2 -75} ]+[expr {$y/2} ]
					grid [ttk::label .top5.l -text "Enter destination "] -row 1 -column 1 -sticky ew
					grid [tk::entry .top5.ent -textvariable ::destination -width 15] -row 1 -column 2 
					grid [ttk::frame .top5.sp1 -height 20] -row 0 -column 1 -columnspan 2
					grid [ttk::frame .top5.sp2 -height 20] -row 2 -column 1 -columnspan 2
					grid [ttk::frame .top5.sp3 -width 15] -row 0 -column 0 -rowspan 2
					grid [ttk::frame .top5.sp4 -width 15] -row 0 -column 3 -rowspan 2
					
					grid [ttk::button .top5.bt -text "Ok" -command " Shortest $tgname ;destroy .top5 "] -row 3 -column 1
					grid [ttk::button .top5.bt2 -text "Cancel" -command "destroy .top5"] -row 3 -column 2 -sticky e
		puts $::destination
	
				focus .top5.ent
}
proc Shortest { tgname } {
	
	puts $tgname
	if {[string match 0node* $tgname]==1} {
		set label[$::f.board itemcget $tgname -text]
	} elseif {[string match 1node* $tgname]==1} {
		set xx [string range $tgname 1 end]
		set label "0$xx"
	}
	puts $label
	set label [$::f.board itemcget $label -text]
	puts $label
	puts $::destination
	set head1 [getHead $::head]
	
	set cue [ShortestPath $label $::destination $::head]
	if { $cue==1 } {
		set msg "NO FILE WAS LOADED"
		} elseif {$cue==2 } {
		     set msg " GRAPH NOT PROPERLY LOADED"
		} elseif { $cue==5 } {
			set msg "SOURCE AND DEST ARE SAME"
		} elseif {$cue==7 } {
			set msg "NO FEASIBLE PATH"
		} elseif {$cue==4} {
			set msg "the destination node name you entered is wrong"
		}
	if { $cue==1 ||$cue==2||$cue==5||$cue==7||$cue==4 } {
			tk_messageBox -message $msg -icon warning -title Caution
	}
			
	image create photo temp -file "./tmp/temp_12345.gif"
				$::vizcanv create image 0 0 -anchor nw -image temp
	
}

proc info1 {} {
		set tgname $::lastVisited
		if {[string match 0node* $tgname]==1} {
		set label[$::f.board itemcget $tgname -text]
	} elseif {[string match 1node* $tgname]==1} {
		set xx [string range $tgname 1 end]
		set label "0$xx"
	}
	puts $label
	set label [$::f.board itemcget $label -text]
	puts $label
	update_num $::head
	set degree [return_info $label $::head]
	set clustering [cluster_interface $::head $label]
	if { $clustering >=0 } {
	set msg1 "The required clustering coefficient is $clustering"
	} else {
		if {$clustering==-1} {
				set D 0
			} else {
			set D 1
			}
		set msg1 "Clustering coefficient can not be found as G(v)=$D" 
	}
	if { $::GraphType eq "Directed" } {
		set msg2 "the indegeree of the node is $degree"
	} else {
		set msg2 "the indegeree of the node is $degree"
	}
	tk_messageBox -message "node name :: $label \n\n $msg2 \n\n $msg1" -icon info -title Information
	
	
}


