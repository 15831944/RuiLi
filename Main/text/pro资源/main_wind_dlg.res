! 28-Apr-97 H-03-09  WHY    $$1    Added ProeWindow component.  
! 09-May-97 H-03-10  WHY    $$2    Added place-holder menu;
!                                  copy other stuff from main_dlg.res.
! 15-May-97 H-03-10  YNS    $$3    Small changes.
! 20-May-97 H-03-11  ked    $$4    Added message display UI.
! 19-May-97 H-03-11  YNS           "Process Info".
! 20-May-97 H-03-11  YNS           "Regen Info", "Analisys"
! 20-May-97 H-03-11  ked           Placed message display UI on bottom.
! 20-May-97 H-03-11  rmi    $$5    Added Mesh Surface to view menu.
! 27-May-97 H-03-12  rmi    $$6    Explode/Unexplode button separated into
!                                  two different buttons
! 23-May-97 H-03-12  ked           Changed help line to a label
! 22-May-97 H-03-12  YNS           Small corrections
! 27-May-97 H-03-12  YNS/ET        Replace psh_import with CascadeButton.
! 27-May-97 H-03-12  ked           temp fix of bug.
! 30-May-97 H-03-13  ked    $$7    UI changes and additions, stop sign, removed
!                                  temp fix of bug.
! 30-May-97 H-03-13  YNS           Changed toolbar
! 03-Jun-97 H-03-13  WHY           Repl. "ProCmdWinClose" w. "ProCmdWinCloseAsyn".
! 02-Jun-97 H-03-13  YNS           UDM.
! 21-May-97 H-03-13  vrs           Utilities->Applications.
! 03-Jun-97 H-03-13  ked           Changed stop sign bif name
! 02-Jun-97 H-03-13  VIK           Added buttons to toolbar.
! 03-Jun-97 H-03-13  YHC           "Trail/Training File ...".
! 03-Jun-97 H-03-13  vrs           Edit & Load Config.
! 03-Jun-97 H-03-13  JMT           Help menu changes.
! 03-Jun-97 H-03-13  YNS           Used "refresh.bif".
! 03-Jun-97 H-03-13  leo           Moved Layers from Utilities to View.
! 03-Jun-97 H-03-13  WHY           Added radio_win_list.
! 06-Jun-97 H-03-14  WHY    $$8    Use place-holders. 
! 12-Jun-97 H-03-14  dbc    $$9    But not for the toolbar - it's defined
!                                  programmatically.  TEMPORARILY put the
!                                  initial toolbar contents here -- this will 
!                                  change soon!
! 19-Jun-97 H-03-14+ YNS    $$10   Typo.
! 20-Jun-97 H-03-15  YNS    $$11   One more + changed buttons style.
! 19-Jun-97 H-03-15  aap           I put move many in the toolbar for drawings
!                                  in H-03-13, but somebody conflict-resolved
!                                  it away!
! 24-Jun-97 H-03-15  ISH           Added "Print"
! 20-Jun-97 H-03-15  YNS           Named Views for toolbar.
!                    YNS/JAS       Used sashes.
! 24-Jun-97 H-03-15  ISH           Added more icons
! 30-Jun-97 H-03-16  ISH    $$12   Removed all icons but one.  Otherwise, 
!                                  the file can not be loaded.
! 30-Jun-97 H-03-16  YNS           Removed "Log" (moved to "Info")
! 03-Jul-97 H-03-16  HMT           Added maximize and minimize buttons
! 04-Jul-97 H-03-16  WHY           Not using pholder for proe window. 
! 13-Jul-97 H-03-17  AW    $$13    Removed erroneous label
! 24-Jul-97 H-03-17  YNS   $$14    AutoRaise, Decorated.
! 14-Jun-97 H-03-17  ISH   $$15    Reworked all toolbar related stuff
! 28-Jul-97 H-03-17  ked   $$16    Changed rows on "txt_messages", no visible 
! 05-Aug-97 H-03-17  ISH   $$17    changed buttons to toolbars
!  2-Sep-97 H-03-20  IlG/dbc $$18  Smaller sash; no insets for Pro/E window; 
!                                  padding around the toolbars.
! 03-Sep-97 H-03-21  ISH   $$19    Added Mapkeys pholder and an additional sash
!                                  to swap graphics and message windows
! 11-Sep-97 H-03-22  ISH   $$20    Added ... to popup menu commands
! 24-Oct-97 H-03-28  ked   $$21    Made stop sign a drawing area
! 23-jan-97 H-03-37+ ked   $$22    set gravity, and selection_visible
! 28-MAR-98 I-01-05   MB   $$23    Added ... and Milling.
! 03-Jul-98 I-01-14  YNS   $$24    Added Toolbox.
! 03-Jul-98 I-01-14  SCL   $$25    Added popups used in user_tb_edit.c
! 06-Aug-98 I-01-16  jas   $$26    Added gravity to sash_swap
! 20-AUG-98 I-01-17  MB    $$27    Added  NCTlbox.
! 01-Sep-98 I-01-18 XKA   $$28  Used UI_POPUPHELP_FONT (2) and 
!				 UI_MESSAGE_FONT(4).
! 15-Sep-98 I-01-20 SCL   $$29  Set .KeyboardInput for tool-style buttons
! 28-Sep-98 I-01-21 DFOM  $$30  Changed Toolbox to Design Tools.
! 12-Oct-98 I-01-22 SCL   $$31  Change #25 to use radio group.
! 23-Oct-98 I-01-26 RCA   $$32  Set remember size/pos to false (bug630515)
! 06-Nov-98 I-01-25 DFOM  $$33  DesignTools->Analysis
! 10-MAR-99 I-03-03 JLM   $$34  Changed toolbar/msg win cosmetics
! 10-Mar-99 I-03-05 PHIGR $$35  Added File and Edit top level menus
! 16-MAR-99 I-03-06 KQ    $$36  Added Datum.
! 09-Apr-99 I-03-07 XKA   $$37  Set FontClass.
! 12-Mar-99 I-03-07 jas/ked $$38 Removed unnecessary stop sign tab
! 14-Apr-99 I-03-07 XKA   $$39  Correctly set UI_MESSAGE_FONT (now 3).
! 23-MAY-99 I-03-10 Hanoch $$40 Moved Windows menu to left of Help menu
! 02-Jun-99 I-03-10 JJE/jas $$41 Add isite logo
! 17-Jun-99 I-03-11 YFR     $$42 Add Rmb popup menu for custom menus
! 17-Jul-99 I-03-12 YNS     $$43 Added ASFEdit, ASFeature.
! 19-Jul-99 I-03-12  YFR    $$44 Change display style options in the popups
! 27-Jul-99 I-03-12 kumar/amin $$45 added changes for animation screen
! 16-Aug-99 I-03-13 RKD     $$46 Adding animation menu panes
! 03-Aug-99 I-03-13 hanoch/zjl $$47  added PHTTop,Bottom, left, right for embed
!                               model tree, + tree menu and action menu.
! 25-Aug-99 I-03-13+ YNS    $$48 Fixed prev.
! 07-Sep-99 I-03-14  zjl    $$49 removed tree menu.
! 08-Sep-99 I-03-15  TRY    $$50 Removed ASFEdit.
! 27-SEP-99 I-03-16  esc    $$51 Added new Freeform (CDRS) menu.
! 28-SEP-99 I-03-16  SHK    $$52 Added Pushbutton FixComp.
! 03-OCT-99 I-03-17  och    $$53 Reorder buttons in ActionMenu
! 01-Oct-99 I-03-17  guy    $$54 Added OleMenu.
! 01-OCT-99 I-03-17  YV     $$55 Added MimicTP.
! 07-OCT-99 I-03-17  RKD    $$56 Changing the location of Animation menu.
! 11-Oct-99 I-03-18  zjl    $$57 Recopied ActionMenu from newtree.res.
! 22-Oct-99 I-03-19 Shturm $$58 Added new Simulation actions.
! 31-OCT-99 I-03-20 Hanoch $$59 Changed menu pane order per Spec.
! 03-DEC-99 I-03-23  YV     $$60 Changed the Lable for Mimic as well as location
! 06-DEC-99 I-03-21  EY     $$61 Removed NCTlbox.
! 03-Dec-99 I-03-23 PMU    $$62 Reordered RMB info actions
! 09-Feb-00 I-03-27 Shturm $$63 Fixed typo.
! 10-Feb-00 J-01-02 RTMN        Change label for CreateMeshCtrl to "New Mesh Control".
! 20-Feb-00 J-01-03 Menn   $$64 Added new moldesign pull-down menus.
! 24-Feb-00 J-01-03 sgur   $$65 Added OpenBase, BaseMdlInfo
! 28-Feb-00 J-01-03 Yossi  $$66 Added the tutorial utility layout and components
! 24-Feb-00 J-01-03 ked    $$67 Added drawing actions to action menu
! 29-Feb-00 J-01-03 Peter  $$68 Added Insert pane.
! 22-Feb-00 J-01-03 lea    $$69 Added Export Geometry.
! 08-Mar-00 J-01-03 yossi  $$70 Changed the TutorialSash to be visible
! 15-Feb-00 J-01-03 Shturm $$71 Removed unused actions for loads/constraints.
! 06-Mar-00 J-01-04 Menn   $$72 Added new mold popups.
! 09-Mar-00 J-01-04 mey    $$73 Added components for Pipe Product Tree 
! 22-Mar-00 J-01-05 mey    $$74 Changed Reset Status to Reset Mode 
! 24-Mar-00 J-01-05 rjt    $$75 Added Toolmaker
! 24-MAR-00 J-01-05 och    $$76 Removed Datum (moved to insert_pane.res).
! 23-Mar-00 J-01-05 Ruth   $$77 Tuto. FlyOut -> TutorialButtons.
! 27-Mar-00 J-01-05 EAF    $$78 Added NewSpring, NewGap, NewMass,
!                               NewSpringProperty, NewMassProperty
! 29-MAR-00 J-01-05 Hanoch $$79 Split window support
! 26-Mar-00 J-01-05 ked    $$80 Added Format, moved Insert
! 03-Apr-00 J-01-06 rjt    $$81 Removed toolmaker
! 07-Apr-00 J-01-06 UK     $$82 Fixed sash gravity settings
! 28-Apr-00 J-01-07 mey    $$83 Added pipe_ins_fit and pipe_del_fit 
! 21-Apr-00 J-01-07 leo    $$84 Added HideFeat_PushBtn, UnhideFeat_PushBtn,
!                               UnhideAllFeat_PushBtn.
! 23-apr-00 J-01-07 rjt    $$85 Added jobmgr
! 25-Apr-00 J-01-07 UKK    $$86 Moved Insert after View to comply Microsoft Std
! 01-May-00 J-01-07 sruiz  $$87 Added Mechanism right mouse button menus for Mechanism model tree
! 26-May-00 J-01-09 Peter  $$88 Added DynModify.
! 20-Jun-00 J-01-11 EAF    $$89 NewMaterialOrientation became CascadeButton.
! 06-Jul-00 J-01-12 RKD    $$90 Mechanism RMB picks modified
! 25-Jun-00 J-01-12 Yossi  $$91 Change the position and the buttons of the TutorialLayout.
! 16-Jun-00 J-01-12 leo    $$92 Removed UnhideAllFeat_PushBtn.
! 30-Jul-00 J-01-13 Yossi  $$93 Redesigned the look of the TutorialLayout.
! 19-JUL-00 J-01-13 akap   $$94 Added wld_convert PushBtn.
! 02-Aug-00 J-01-14 ked    $$95 Moved Format before Analysis
! 17-Aug-00 J-01-16 olegi  $$96 Removed ExportGeom.
! 16-Aug-00 J-01-16 Yossi  $$97 Move the TutorialLayout outside this resource into a place holder.
! 08-Aug-00 J-02-01 Mey    $$98 Added piping component and spool views
! 01-Dec-00 J-01-22 auk    $$99 changed PopupCBRegardless attribute of
!                               ActionMenu using GUIDE
! 09-Nov-00 J-02-02 RTMN   $$100 Added new sim actions.
! 19-Jan-01 J-02-03 PSG    $$101 Added MakeIndependent.  
! 05-Jan-01 J-01-25 pmu    $$102 Updated labels in ActionMenu
! 09-Feb-01 J-02-04 Mey    $$103 Added Create/Delete Insulation
! 13-Mar-01 J-02-06 Mark   $$104 Added Promote button.
! 05-Feb-01         jas         Fixed sizing problems
! 15-Feb-01 J-01-27 jas    100 Fixed CmdPopupMenu popup action problems
! 01-Mar-01 J-01-28 UK     101 Fixed initial size problems
! 15-Mar-01 J-01-30 ked    102 Added help message for stop sign
! 26-Mar-01 J-01-30 jas    103 Fixed further initial size problems
! 26-Mar-01 J-01-30 pmu    104 Updated bif for "Reset Mode"
! 08-JUN-01 J-03-01 integ  $$105 J-02 submission
! 31-May-01 J-03-01+ Idan  $$106 Removed FileName and ResName attribs  from
!                                Layout PHPHTLeft, PHPHTRight, PHPHTTop 
!                                and PHPHTBottom
! 19-JUN-01 J-03-01 integ  $$107 Merge fix
! 29-May-01 J-03-02 Moti   $$108 Removed sash_right , SashTreeBottom , 
!				 PHPHTTop , PHPHTRight .
! 07-Jun-01 J-03-01 sgur   $$109 Added UpdateInheritance.
! 05-Jul-01 J-03-03 Ruth   $$110 Support dashboard.
! 08-Jul-01 J-03-03 RTMN   $$111 Added new sim actions: NewWLink, NewRLink.
! 13-May-01 J-03-03  RUB   $$112 Added MakeActive.
! 18-Jul-01 J-03-04 Shturm $$113 Fixed #111
! 10-Jul-01 J-03-04 Moti   $$114 removed SashTTop , PHPHTBottom .
! 25-Jul-01 J-03-05 Moti   $$115 Removed upper_sep_tab , and lower_sep_tab .
! 07-Aug-01 J-03-05  RSS   $$116 Added Cancel.
! 13-Aug-01 J-03-06  MBV   $$117 Added Status bar "push_btn_bin".
! 10-Aug-01 J-03-06 Shturm $$118 Added New Connections menu.
! 29-Aug-01 J-03-07 Moti   $$119 Added AddFavMenuPane menu .
! 10-Sep-01 J-03-07 sgur   $$120 Added RemoveVarItems.
! 07-Aug-01 J-03-08 Vaza   $$121 Added ATB.
! 10-Oct-01 J-03-10 Moti   $$122 Added functionality to FavTabMenuPane .
! 19-Oct-01 J-03-11 xw     $$123 Added AddRefBalloon AddBomBalloon.
! 22-Oct-01 J-03-11 Alex   $$124 Change .SashWidth of sash_left to 0 
! 29-Oct-01 J-03-11 AEY	   $$125 Modified HelpText and Label for ATB_unlink_auto
!				 and ATB_relink.
! 07-Nov-01 J-03-12 Vaza   $$126 Modified Label/HelpText for 
!                                ATB_ver/upd/unlink_auto.
! 07-Nov-01 J-03-12 Alex   $$127 Restore .SashWidth of sash_left to 3 ( ##124 ) 
!                                 add GraphicsDrawingArea
! 15-Nov-01 J-03-12 AS	   $$128 Added PmInfoSummary and PmInfoDetails in action menu
! 21-Nov-01 J-03-12+AW     $$129 Undo addition of GraphicsDrawingArea
! 13-Nov-01 J-03-13 Moti   $$130 Added HistoryTabMenuPane .
! 22-Nov-01 J-03-13 abera  $$131 Added changes for Mechanism screen
! 27-Nov-01 J-03-13 PBP    $$132 Added EditModify.
! 27-Nov-01 J-03-14 Vaza   $$133 Removed ActionSep and ATBSeparator.
! 10-Dec-01 J-03-14 Moti   $$134 Added progress bar on Sun .
! 29-Nov-01 J-03-14 elevy  $$135 Added Rename button.
! 24-Dec-01 J-03-15 hanoch $$136 Changed browser sash .
! 26-Dec-01 J-03-15 VRI    $$137 Removed psh_btn_bin
! 02-Dec-01 J-03-15 RUB    $$138 Added RegenAuto & RegenCust.
! 30-Dec-01 J-03-16 Moti   $$139 Added PHTTop , SashTTop .
! 10-Jan-02 J-03-17 YNS    $$140 Added Restyle.
! 15-Jan-02 J-03-17 Hanoch $$141 switch SashTTop with TreeSash .
! 16-Jan-02 J-03-17 dea    $$142 Added Make Flexible & Flexible Component.
! 13-JAN-02 J-03-17 akap   $$143 Added actions for extract assem comps.
! 14-Jan-02 J-03-17 JJE    $$144 Add Sendto, Remove isite logo
! 17-Jan-02 J-03-17 RSS    $$145 Added EditPb to UpdatePb.
! 15-Jan-02 J-03-17 JAM    $$146 Table top menu added for Drawing UI.
! 18-JAN-02 J-03-17 integ  $$147 Correct errors
! 18-JAN-02 J-03-17 integ  $$148 Correct more errors.
! 27-Jan-02 J-03-18 AlexM  $$149 Fix CmdPopupMenu
! 30-Jan-02 J-03-18 pak    $$150 Added GlobalModel menu.
! 31-Jan-02 J-03-18 RSS    $$151 Modified help text and label for EditPb,
!                                EditCancelPb and UpdatePb.Removed WildSep.
! 31-Jan-02 J-03-18 BDS    $$152 Added Points/Wrap etc.
! 15-Feb-02 J-03-19 RSS    $$153 Modified label for EditCancelPb and DetailedPb
! 11-MAR-02 J-03-20+Hanoch $$154 Removed redunant layout in status bar
! 11-Mar-02 J-03-21 UK     $$155 Fixed sash gravity yet again (who broke it ?)
! 12-Apr-02 J-03-23 dea    $$156 InsFlexComp moved to "Component" cascade menu.
! 04-Feb-02 J-03-23 AlexM  $$157 Add GraphicsNakedWindow  
! 26-Apr-02 J-03-24 adubey $$158 Modified label for CheckinCasecade.
! 07-May-02 J-03-24 JJE    $$159 Change button label to send as zipped attachment
! 28-May-02 J-03-26 AmitG  $$160 Added Action Menu for Thermal Modeler(TM).
! 28-May-02 J-03-26  RSS   $$161 Modified label for EditPb, EditCancelPb,
!                                UpdatePb and ExpresPb.
! 24-May-02 J-03-26 RUB    $$162 Added RegenPart.
! 07-Jun-02 J-03-27 PBP    $$163 Added EditProperties.
! 18-Jun-02 J-03-28 RUB    $$164 Added EditDtmTag.
! 20-Jun-02 J-03-28  DK    $$165 Added AddURL, EditURL and RemoveURL.
! 08-Jun-02 J-03-29 ASIT   $$166 Removed TT_COMP_EDIT entry.
! 03-Jul-02 J-03-29 vpu    $$167 Added UpdateAltasm.
! 11-JUL-02 J-03-29  akap        changed ForReference.Label.
! 14-Jul-02 J-03-29 DYR    $$168 Added GRVInfo button.
! 02-Aug-02 J-03-31 xw     $$169 Generate BOM Balloon => Create BOM Balloon
! 20-Aug-02 J-03-32 RUB    $$170 Added EditSecInd.
!                                Made changes in help texts.
! 04-Sep-02 J-03-33 RSS    $$171 Modified label and help text for EditCancelPb.
! 02-Sep-02 J-03-33 ASIT   $$172 Removed TT_COMP_NAME entry.
! 24-Sep-02 J-03-34  DK    $$173 Added DeleteSymbol.
! 30-Sep-02 J-03-35 vpu    $$174 Added ExitEnvelope.
! 30-Sep-02 J-03-35 Moti   $$175 Added CheckDefaultButton attribute.
! 07-Nov-02 J-03-37 elevy  $$176 Removed DynModify button.
! 02-Jan-03 J-03-40 RSS    $$177 Modified Label and Help Text for UpdateAltasm.
! 07-Jan-01 J-03-40 Moti   $$178 Removed KeyboardInput from temp buttons .
! 31-Dec-02 K-01-01 AlexM  $$179 4-views project 
! 21-Jan-03 K-01-01 ASIT   $$180 Added Mesh and Properties for simulation.
! 20-Feb-03 K-01-01 VAD    $$181 Added Ncsubnow, Ncsublater, Ncplaypath.
! 06-Mar-03 K-01-02 Asaf   $$182 Removed Tutorial Sash.
! 09-Mar-03 K-01-02 sgur   $$183 Added PropagateFlexTo.
! 25-Feb-03 K-01-02 ASIT   $$184 Removed Simulation TT_COMP_NEW_.. entries.
! 09-Mar-03 K-01-02 AlexM  $$185 Added DashboardSash
! 14-Mar-03 K-01-03 ASIT   $$186 Moved FEMObjInfo out from Info cascade.
! 23-Mar-03 K-01-03 ElSh   $$187 Add Annot. Element Command Buttons.
! 12-Mar-03 K-01-03 AlexM  $$188 Changed DrawingArea.DoubleBuffered to False 
! 04-Apr-03 K-01-04 SAY    $$189 Added PmObjActMenu MenuPane.
! 26-Mar-03 K-01-04 stao   $$190 Changed Sec Indep to Feat Ind 
! 28-Mar-03 K-01-04 rkd    $$191 Material model tree RMB menu added.
! 12-Mar-03 K-01-05 RSS    $$192 Added Diagnose menu.
! 12-Mar-03         KKU          Added Clip Suppress and Make Indep menu.
! 05-May-03 K-01-06 AKL    $$193 Added FtFormReplace.
! 28-Apr-03 K-01-06 ElSh         Removed AEModify, AEDelete command.
! 28-Apr-03 K-01-06 vpu          Added WeldChangeRod/Proc, WeldModParams butns
! 01-May-03 K-01-06 dzel   $$194 Added PutToFooter, GetFromFooter.
! 24-Apr-03 K-01-06 VRI    $$195 Added sstbar_popup.
! 11-May-03 K-01-07 vpu    $$196 Added ComponentPath.
! 06-Mar-03 K-01-07 Moti   $$197 Moved ActionMenu to main_action_menu.res
! 10-Jun-03 K-01-08 jas    $$198 Use UI PGLWindow class for graphics area
! 17-Oct-03 K-01-16+AW     $$199 Removed msg_layout
! 02-Nov-03 K-01-17 Moti   $$200 Added keyboard input to proe_win.
! 11-Dec-03 K-01-20 AlexM  $$201 Set lower_sep.Visible to False
! 05-Feb-04 K-01-23 jas    $$202 Reinstated Pro/E dialog icons
!
! Do not forget to put "!" at the begining of each line in this comment block.

#include <main_action_menu.res>

(Dialog main_wind_dlg
    (Components
        (MenuBar                        MenuBar1
                                        File
                                        Edit
                                        View
                                        Insert
                                        Points
                                        Wrap
                                        Ftune
                                        Facet
                                        Sketch
                                        Animation
                                        Mechanism
                                        ASFeature
                                        Restyle
                                        Freeform
                                        GlobalModel
                                        Mfgstp
                                        Milling
                                        Table
                                        Format
                                        Properties
                                        Mesh
                                        AutoGEM
                                        Analysis
                                        Info
                                        Applications
                                        Utilities
                                        Mldmfg
                                        Mldassem
                                        Mapkeys
                                        Windows
                                        Help)
        (PHolder                        File)
        (PHolder                        Edit)
        (PHolder                        View)
        (PHolder                        Insert)
        (PHolder                        Points)
        (PHolder                        Wrap)
        (PHolder                        Ftune)
        (PHolder                        Facet)
        (PHolder                        Sketch)
        (PHolder                        Animation)
        (PHolder                        Mechanism)
        (PHolder                        ASFeature)
        (PHolder                        Restyle)
        (PHolder                        Freeform)
        (PHolder                        GlobalModel)
        (PHolder                        Mfgstp)
        (PHolder                        Milling)
        (PHolder                        Table)
        (PHolder                        Format)
        (PHolder                        Properties)
        (PHolder                        Mesh)
        (PHolder                        AutoGEM)
        (PHolder                        Analysis)
        (PHolder                        Info)
        (PHolder                        Applications)
        (PHolder                        Utilities)
        (PHolder                        Mldmfg)
        (PHolder                        Mldassem)
        (PHolder                        Mapkeys)
        (PHolder                        Windows)
        (PHolder                        Help)
        (SashLayout                     sash_top)
    )

    (Resources
        (MenuBar1.VisibleNames          "File")
        (.Bitmap                        "proe")
        (.Label                         "Pro/E")
        (.DialogStyle                   5)
        (.StartLocation                 1)
        (.PlaceHolderMenu               "win_dlg_menu")
        (.MaximizeButton                True)
        (.MinimizeButton                True)
        (.OverlayPlane                  False)
        (.RememberPosition              False)
        (.AutoRaise                     False)
        (.RememberSize                  False)
        (.TitleBarImage                 "proe_small")
        (.CheckDefaultButton            False)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                sash_top
            )
        )
    )
)

(Sash sash_top
    (Components
        (SubToolBar                     pro_toolbar_seed_top)
        (SashLayout                     sash_bottom)
    )

    (Resources
        (.Sensitive                     False)
        (.Orientation                   True)
        (.SashPosition                  0)
        (.SashWidth                     0)
        (.Gravity                       1)
    )
)

(ToolBar pro_toolbar_seed_top
    (Components
        (SubToolBar                     temp1)
    )

    (Resources
        (.PopupMenu                     "toolbar_popup_top")
    )
)

(ToolBar temp1
    (Components
        (PushButton                     temp_button1)
    )

    (Resources
        (temp_button1.Bitmap            "empty20x20")
        (temp_button1.ButtonStyle       3)
        (temp_button1.KeyboardInput     False)
    )
)

(Sash sash_bottom
    (Components
        (SashLayout                     SashTTop)
        (SubLayout                      lay_status_line)
    )

    (Resources
        (.Sensitive                     False)
        (.Orientation                   True)
        (.SashPosition                  100)
        (.SashWidth                     0)
        (.Gravity                       9)
    )
)

(Sash SashTTop
    (Components
        (SubLayout                      PHPHTTop)
        (SashLayout                     sash_swap)
    )

    (Resources
        (.Orientation                   True)
        (.SashWidth                     3)
        (.Gravity                       1)
    )
)

(Layout PHPHTTop
    (Components
        (SubLayout                      PHTTop)
        (PHolder                        PHTTop)
    )

    (Resources
        (.Visible                       False)
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.LeftOffset                    2)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                PHTTop
            )
        )
    )
)

(Sash sash_swap
    (Components
        (SubLayout                      lay_message_area)
        (SashLayout                     TreeSash)
    )

    (Resources
        (.Orientation                   True)
        (.SashWidth                     3)
        (.Gravity                       1)
    )
)

(Layout lay_message_area
    (Components
        (Separator                      upper_sep)
        (Separator                      lower_sep)
        (SashLayout                     DashboardSash)
    )

    (Resources
        (upper_sep.TopOffset            0)
        (upper_sep.BottomOffset         0)
        (upper_sep.LeftOffset           0)
        (upper_sep.RightOffset          0)
        (lower_sep.Visible              False)
        (lower_sep.TopOffset            0)
        (lower_sep.BottomOffset         0)
        (lower_sep.LeftOffset           0)
        (lower_sep.RightOffset          0)
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.Layout
            (Grid (Rows 0 1 0) (Cols 1)
                upper_sep
                DashboardSash
                lower_sep
            )
        )
    )
)

(Sash DashboardSash
    (Components
        (TextArea                       txt_messages)
        (SubLayout                      msg_container)
    )

    (Resources
        (txt_messages.Rows              1)
        (txt_messages.Columns           30)
        (txt_messages.MaxLen            200000)
        (txt_messages.Editable          False)
        (txt_messages.Decorated         False)
        (txt_messages.SelectionVisible  True)
        (txt_messages.ScrollBarPosition 6)
        (txt_messages.CaretVisible      False)
        (txt_messages.Gravity           7)
        (txt_messages.FontClass         3)
        (.Sensitive                     False)
        (.Orientation                   True)
        (.SashWidth                     0)
        (.Gravity                       9)
    )
)

(Layout msg_container
    (Components
        (Label                          lbl_container)
    )

    (Resources
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.LeftOffset                    3)
        (.RightOffset                   3)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                lbl_container
            )
        )
    )
)

(Sash TreeSash
    (Components
        (SubToolBar                     pro_toolbar_seed_left)
        (SashLayout                     MainWInSash)
    )

    (Resources
        (.SashPosition                  10)
        (.SashWidth                     0)
        (.Gravity                       1)
    )
)

(ToolBar pro_toolbar_seed_left
    (Components
        (SubToolBar                     temp3)
    )

    (Resources
        (.Visible                       False)
        (.Orientation                   True)
        (.PopupMenu                     "toolbar_popup_left")
        (.TopOffset                     0)
        (.BottomOffset                  0)
        (.LeftOffset                    0)
        (.RightOffset                   0)
    )
)

(ToolBar temp3
    (Components
        (PushButton                     temp_button3)
    )

    (Resources
        (temp_button3.Bitmap            "empty20x20")
        (temp_button3.ButtonStyle       2)
        (temp_button3.KeyboardInput     False)
        (.Orientation                   True)
    )
)

(Sash MainWInSash
    (Components
        (SashLayout                     sash_left)
        (SubToolBar                     pro_toolbar_seed_right)
    )

    (Resources
        (.SashPosition                  10)
        (.SashWidth                     0)
        (.Gravity                       9)
    )
)

(Sash sash_left
    (Components
        (SubLayout                      PHPHTLeft)
        (SashLayout                     anim_sash)
    )

    (Resources
        (.SashPosition                  0)
        (.SashWidth                     0)
        (.Gravity                       1)
    )
)

(Layout PHPHTLeft
    (Components
        (SubLayout                      PHTLeft)
        (PHolder                        PHTLeft)
    )

    (Resources
        (.Visible                       False)
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.LeftOffset                    0)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                PHTLeft
            )
        )
    )
)

(Sash anim_sash
    (Components
        (DrawingArea                    GraphicsDrawingArea)
        (SubLayout                      AnimationLayout)
    )

    (Resources
        (GraphicsDrawingArea.Decorated  False)
        (GraphicsDrawingArea.AttachLeft True)
        (GraphicsDrawingArea.AttachRight True)
        (GraphicsDrawingArea.AttachTop  True)
        (GraphicsDrawingArea.AttachBottom True)
        (GraphicsDrawingArea.DrawingWidth 1000)
        (GraphicsDrawingArea.DrawingHeight 1000)
        (GraphicsDrawingArea.BackgroundColor 20)
        (GraphicsDrawingArea.DoubleBuffered False)
        (.Sensitive                     False)
        (.Orientation                   True)
        (.SashWidth                     3)
        (.Gravity                       9)
    )
)

(DrawingAreaLayout GraphicsDrawingArea
    (Components
        (DrawingArea                    GraphicsDrawingAreaNavigate)
        (PGLWindow                      proe_win)
    )

    (Resources
        (GraphicsDrawingAreaNavigate.Visible False)
        (GraphicsDrawingAreaNavigate.Decorated False)
        (GraphicsDrawingAreaNavigate.AttachLeft True)
        (GraphicsDrawingAreaNavigate.AttachTop True)
        (GraphicsDrawingAreaNavigate.DrawingWidth 1)
        (GraphicsDrawingAreaNavigate.DrawingHeight 1)
        (GraphicsDrawingAreaNavigate.KeyboardInput True)
        (GraphicsDrawingAreaNavigate.ForegroundColor 26)
        (GraphicsDrawingAreaNavigate.BackgroundColor 20)
        (GraphicsDrawingAreaNavigate.DoubleBuffered False)
        (proe_win.Decorated             False)
        (proe_win.KeyboardInput         True)
        (proe_win.Cursor                "UI arrow cursor image")
        (proe_win.NakedMinWidth         1)
        (proe_win.NakedMinHeight        1)
        (proe_win.PGLCallbacks          True)
    )
)



(Layout AnimationLayout
    (Components
        (DrawingArea                    comp_timeline)
        (DrawingArea                    time_scale)
        (ScrollBar                      horizontal_scroll)
        (ScrollBar                      vertical_scroll)
    )

    (Resources
        (comp_timeline.AttachLeft       True)
        (comp_timeline.AttachRight      True)
        (comp_timeline.AttachTop        True)
        (comp_timeline.AttachBottom     True)
        (comp_timeline.Tracking         True)
        (comp_timeline.DrawingHeight    50)
        (comp_timeline.KeyboardInput    True)
        (comp_timeline.BackgroundColor  7)
        (time_scale.Decorated           False)
        (time_scale.AttachLeft          True)
        (time_scale.AttachRight         True)
        (time_scale.AttachTop           True)
        (time_scale.AttachBottom        True)
        (time_scale.Tracking            True)
        (time_scale.DrawingHeight       30)
        (time_scale.BackgroundColor     7)
        (horizontal_scroll.AttachLeft   True)
        (horizontal_scroll.AttachRight  True)
        (horizontal_scroll.Tracking     True)
        (vertical_scroll.Orientation    True)
        (vertical_scroll.AttachTop      True)
        (vertical_scroll.AttachBottom   True)
        (vertical_scroll.KeyboardInput  True)
        (.Visible                       False)
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.Layout
            (Grid (Rows 1 0 0) (Cols 1 0)
                comp_timeline
                vertical_scroll
                time_scale
                (Pos 3 1)
                horizontal_scroll
            )
        )
    )
)



(ToolBar pro_toolbar_seed_right
    (Components
        (SubToolBar                     temp4)
    )

    (Resources
        (.Visible                       False)
        (.Orientation                   True)
        (.PopupMenu                     "toolbar_popup_right")
    )
)

(ToolBar temp4
    (Components
        (PushButton                     temp_button4)
    )

    (Resources
        (temp_button4.Bitmap            "empty20x20")
        (temp_button4.ButtonStyle       2)
        (temp_button4.KeyboardInput     False)
        (.Orientation                   True)
    )
)

(Layout lay_status_line
    (Components
        (SashLayout                     status_line_sash)
        (SubLayout                      lay_stop)
    )

    (Resources
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.Layout
            (Grid (Rows 1) (Cols 1 0)
                status_line_sash
                lay_stop
            )
        )
    )
)

(Sash status_line_sash
    (Components
        (Label                          lbl_help)
        (SubToolBar                     status_bar_main_toolbar)
    )

    (Resources
        (lbl_help.Label                 " ")
        (lbl_help.AttachLeft            True)
        (lbl_help.AttachRight           True)
        (lbl_help.AttachTop             True)
        (lbl_help.AttachBottom          True)
        (lbl_help.Alignment             0)
        (lbl_help.FontClass             2)
        (.Sensitive                     False)
        (.SashPosition                  50)
        (.SashWidth                     0)
        (.Gravity                       9)
    )
)

(ToolBar status_bar_main_toolbar
    (Components
        (SubLayout                      status_bar_main_layout)
    )

    (Resources
        (.Rows                          1)
        (.AttachRight                   False)
        (.AttachTop                     False)
        (.AttachBottom                  False)
        (.Resizeable                    True)
    )
)

(Layout status_bar_main_layout
    (Components
        (Label                          status_bar_label)
    )

    (Resources
        (status_bar_label.Bitmap        "empty10x12")
        (status_bar_label.Visible       False)
        (status_bar_label.Columns       38)
        (status_bar_label.AttachLeft    True)
        (status_bar_label.AttachRight   True)
        (status_bar_label.Alignment     0)
        (status_bar_label.Resizeable    True)
        (.DecoratedStyle                5)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                status_bar_label
            )
        )
    )
)

(Layout lay_stop
    (Components
        (DrawingArea                    psh_stop)
    )

    (Resources
        (psh_stop.HelpText              "Interrupt the current action")
        (psh_stop.Decorated             False)
        (psh_stop.Images                "up_stop_sign"
                                        "dn_stop_sign")
        (psh_stop.DrawingWidth          30)
        (psh_stop.DrawingHeight         22)
        (psh_stop.BackgroundColor       9)
        (.Visible                       False)
        (.AttachRight                   True)
        (.LeftOffset                    2)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                psh_stop
            )
        )
    )
)


(MenuPane toolbar_popup_top
    (Components
        (Label                          label_popup_top)
        (Separator                      label_separator_popup_top)
        (Separator                      separator_popup_top)
        (PushButton                     customize_popup_top)
        (PushButton                     toolbars_popup_top)
    )

    (Resources
        (label_popup_top.Label          "Top Toolchest")
        (label_separator_popup_top.TopOffset 0)
        (label_separator_popup_top.BottomOffset 0)
        (label_separator_popup_top.LeftOffset 0)
        (label_separator_popup_top.RightOffset 0)
        (separator_popup_top.TopOffset  0)
        (separator_popup_top.BottomOffset 0)
        (separator_popup_top.LeftOffset 0)
        (separator_popup_top.RightOffset 0)
        (customize_popup_top.Label      "Commands...")
        (customize_popup_top.HelpText   "Add/Remove commands from toolbars")
        (customize_popup_top.ButtonStyle 2)
        (customize_popup_top.KeyboardInput True)
        (customize_popup_top.ActivateCommand "ProCmdUtilToolbarCustomize")
        (toolbars_popup_top.Label       "Toolbars...")
        (toolbars_popup_top.HelpText    "Show/Hide toolbars")
        (toolbars_popup_top.ButtonStyle 2)
        (toolbars_popup_top.KeyboardInput True)
        (toolbars_popup_top.ActivateCommand "ProCmdUtilToolbarEdit")
        (.PopupMenu                     "toolbar_popup_top")
        (.PopupCBRegardless             1)
    )
)

(MenuPane toolbar_popup_left
    (Components
        (Label                          label_popup_left)
        (Separator                      label_separator_popup_left)
        (Separator                      separator_popup_left)
        (PushButton                     customize_popup_left)
        (PushButton                     toolbars_popup_left)
    )

    (Resources
        (label_popup_left.Label         "Left Toolchest")
        (label_separator_popup_left.TopOffset 0)
        (label_separator_popup_left.BottomOffset 0)
        (label_separator_popup_left.LeftOffset 0)
        (label_separator_popup_left.RightOffset 0)
        (separator_popup_left.TopOffset 0)
        (separator_popup_left.BottomOffset 0)
        (separator_popup_left.LeftOffset 0)
        (separator_popup_left.RightOffset 0)
        (customize_popup_left.Label     "Commands...")
        (customize_popup_left.HelpText  "Add/Remove commands from toolbars")
        (customize_popup_left.ButtonStyle 2)
        (customize_popup_left.KeyboardInput True)
        (customize_popup_left.ActivateCommand "ProCmdUtilToolbarCustomize")
        (toolbars_popup_left.Label      "Toolbars...")
        (toolbars_popup_left.HelpText   "Show/Hide toolbars")
        (toolbars_popup_left.ButtonStyle 2)
        (toolbars_popup_left.KeyboardInput True)
        (toolbars_popup_left.ActivateCommand "ProCmdUtilToolbarEdit")
        (.PopupMenu                     "toolbar_popup_left")
        (.PopupCBRegardless             1)
    )
)

(MenuPane toolbar_popup_right
    (Components
        (Label                          label_popup_right)
        (Separator                      label_separator_popup_right)
        (Separator                      separator_popup_right)
        (PushButton                     customize_popup_right)
        (PushButton                     toolbars_popup_right)
    )

    (Resources
        (label_popup_right.Label        "Right Toolchest")
        (label_separator_popup_right.TopOffset 0)
        (label_separator_popup_right.BottomOffset 0)
        (label_separator_popup_right.LeftOffset 0)
        (label_separator_popup_right.RightOffset 0)
        (separator_popup_right.TopOffset 0)
        (separator_popup_right.BottomOffset 0)
        (separator_popup_right.LeftOffset 0)
        (separator_popup_right.RightOffset 0)
        (customize_popup_right.Label    "Commands...")
        (customize_popup_right.HelpText "Add/Remove commands from toolbars")
        (customize_popup_right.ButtonStyle 2)
        (customize_popup_right.KeyboardInput True)
        (customize_popup_right.ActivateCommand "ProCmdUtilToolbarCustomize")
        (toolbars_popup_right.Label     "Toolbars...")
        (toolbars_popup_right.HelpText  "Show/Hide toolbars")
        (toolbars_popup_right.ButtonStyle 2)
        (toolbars_popup_right.KeyboardInput True)
        (toolbars_popup_right.ActivateCommand "ProCmdUtilToolbarEdit")
        (.PopupMenu                     "toolbar_popup_right")
        (.PopupCBRegardless             1)
    )
)

(MenuPane CmdPopupMenu
    (Components
        (PushButton                     CmdDeleteItem)
        (Separator                      CmdPopupMenuSep1)
        (PushButton                     CmdCopyImage)
        (Separator                      CmdPopupMenuSep2)
        (RadioGroup                     CmdDisplayType)
        (PushButton                     CmdWhatsThis)
    )

    (Resources
        (CmdPopupMenuSep1.TopOffset     0)
        (CmdPopupMenuSep1.BottomOffset  0)
        (CmdPopupMenuSep1.LeftOffset    0)
        (CmdPopupMenuSep1.RightOffset   0)
        (CmdPopupMenuSep2.TopOffset     0)
        (CmdPopupMenuSep2.BottomOffset  0)
        (CmdPopupMenuSep2.LeftOffset    0)
        (CmdPopupMenuSep2.RightOffset   0)
        (CmdDisplayType.Names           "TextOnly"
                                        "ImageOnly"
                                        "ImageText")
        (CmdDisplayType.Labels          ""
                                        ""
                                        "")
        (CmdWhatsThis.Label             "What's This?")
        (CmdWhatsThis.HelpText          "Tells what this button does")
        (.PopupCBRegardless             1)
    )
)

(MenuPane MapkeyPopupMenu
    (Components
        (PushButton                     MapkeyDeleteItem)
        (Separator                      Separator2)
        (PushButton                     MapkeyCopyImage)
        (PushButton                     MapkeyPasteImage)
        (PushButton                     MapkeyResetImage)
        (PushButton                     MapkeyChooseImage)
        (PushButton                     MapkeyEditImage)
        (Separator                      Separator3)
        (RadioGroup                     MapkeyDisplayType)
        (PushButton                     MapkeyWhatsThis)
    )

    (Resources
        (Separator2.TopOffset           0)
        (Separator2.BottomOffset        0)
        (Separator2.LeftOffset          0)
        (Separator2.RightOffset         0)
        (Separator3.TopOffset           0)
        (Separator3.BottomOffset        0)
        (Separator3.LeftOffset          0)
        (Separator3.RightOffset         0)
        (MapkeyDisplayType.Names        "TextOnly"
                                        "ImageOnly"
                                        "ImageText")
        (MapkeyDisplayType.Labels       ""
                                        ""
                                        "")
        (MapkeyWhatsThis.Label          "What's This?")
        (MapkeyWhatsThis.HelpText       "Tells what this button does")
        (.PopupCBRegardless             1)
    )
)

(MenuPane cmPopupMenu
    (Components
        (PushButton                     cmDeleteItem)
        (Separator                      Separator9)
        (PushButton                     cmName)
        (PushButton                     cmCopyImage)
        (PushButton                     cmPasteImage)
        (PushButton                     cmResetImage)
        (PushButton                     cmChooseImage)
        (PushButton                     cmEditImage)
        (Separator                      Separator10)
        (RadioGroup                     cmDisplayType)
        (PushButton                     cmWhatsThis)
        (Separator                      Separator11)
        (CheckButton                    cmBgnGrp)
    )

    (Resources
        (Separator9.TopOffset           0)
        (Separator9.BottomOffset        0)
        (Separator9.LeftOffset          0)
        (Separator9.RightOffset         0)
        (cmName.Label                   "Rename")
        (cmName.HelpText                "Rename menu/cascade menu")
        (Separator10.TopOffset          0)
        (Separator10.BottomOffset       0)
        (Separator10.LeftOffset         0)
        (Separator10.RightOffset        0)
        (cmDisplayType.Names            "TextOnly"
                                        "ImageText")
        (cmDisplayType.Labels           ""
                                        "")
        (cmWhatsThis.Label              "What's This?")
        (cmWhatsThis.HelpText           "Tells what this button does")
        (Separator11.TopOffset          0)
        (Separator11.BottomOffset       0)
        (Separator11.LeftOffset         0)
        (Separator11.RightOffset        0)
        (cmBgnGrp.Label                 "Begin a Group")
        (cmBgnGrp.HelpText              "Add a group separator before this menu command")
        (.PopupCBRegardless             1)
    )
)

(MenuPane pop_drv_mnu
    (Components
        (PushButton                     edit_drv_inst)
        (PushButton                     edit_def_drv)
        (PushButton                     copy_drv)
        (PushButton                     del_inst_drv)
        (PushButton                     sel_ref_ent_drv)
    )

    (Resources
        (edit_drv_inst.Label            "Edit Time")
        (edit_def_drv.Label             "Edit Servo Motor")
        (copy_drv.Label                 "Copy")
        (del_inst_drv.Label             "Remove")
        (sel_ref_ent_drv.Label          "Select ref entity")
    )
)

(MenuPane pop_common_mnu
    (Components
        (PushButton                     edit_def)
        (PushButton                     copy)
        (PushButton                     del_inst)
        (PushButton                     sel_ref_ent)
    )

    (Resources
        (edit_def.Label                 "Edit")
        (copy.Label                     "Copy")
        (del_inst.Label                 "Remove")
        (sel_ref_ent.Label              "Select ref entity")
    )
)

(MenuPane pop_delete_mnu
    (Components
        (PushButton                     del_multiple)
    )

    (Resources
        (del_multiple.Label             "Remove")
    )
)

(MenuPane pop_kfs_mnu
    (Components
        (PushButton                     edit_kfs_inst)
        (PushButton                     edit_def_kfs)
        (PushButton                     copy_kfs)
        (PushButton                     del_inst_kfs)
        (PushButton                     sel_ref_ent_kfs)
    )

    (Resources
        (edit_kfs_inst.Label            "Edit Time")
        (edit_def_kfs.Label             "Edit KFS")
        (copy_kfs.Label                 "Copy")
        (del_inst_kfs.Label             "Remove")
        (sel_ref_ent_kfs.Label          "Select ref entity")
    )
)

(MenuPane pop_zoom_mnu
    (Components
        (PushButton                     zoom_in_anim)
        (PushButton                     zoom_out_anim)
        (PushButton                     refit_anim)
        (PushButton                     edit_time_domain)
    )

    (Resources
        (zoom_in_anim.Label             "Zoom in")
        (zoom_out_anim.Label            "Zoom out")
        (refit_anim.Label               "Refit")
        (edit_time_domain.Label         "Edit Time domain")
    )
)

(MenuPane OleMenu
    (Components
        (PushButton                     Ole1)
        (PushButton                     Ole2)
        (PushButton                     Ole3)
    )

    (Resources
        (Ole1.Label                     "Open")
        (Ole2.Label                     "Edit")
        (Ole3.Label                     "Delete")
    )
)

(MenuPane FavTabMenuPane
    (Components
        (PushButton                     TabMenuExpandAllPshBtn)
        (PushButton                     TabMenuCollapseAllPshBtn)
        (Separator                      TabMenuSep1)
        (PushButton                     TabMenuDeletePshBtn)
        (PushButton                     TabMenuRenamePshBtn)
        (PushButton                     TabMenuActivateLinkPshBtn)
        (PushButton                     TabMenuExpandPshBtn)
        (PushButton                     TabMenuCollapsePshBtn)
    )

    (Resources
        (TabMenuExpandAllPshBtn.Label   "Expand All")
        (TabMenuCollapseAllPshBtn.Label "Collapse All")
        (TabMenuSep1.TopOffset          0)
        (TabMenuSep1.BottomOffset       0)
        (TabMenuSep1.LeftOffset         0)
        (TabMenuSep1.RightOffset        0)
        (TabMenuDeletePshBtn.Label      "Delete")
        (TabMenuRenamePshBtn.Label      "Rename")
        (TabMenuActivateLinkPshBtn.Label "Go To ...")
        (TabMenuExpandPshBtn.Label      "Expand")
        (TabMenuCollapsePshBtn.Label    "Collapse")
    )
)

(MenuPane HistoryTabMenuPane
    (Components
        (PushButton                     HistoryTabMenuOpenPshBtn)
        (PushButton                     HistoryTabMenuClosePshBtn)
        (PushButton                     HistoryTabMenuAddToFavPshBtn)
        (Separator                      HistoryTabMenuSep1)
        (PushButton                     HistoryTabMenuDeletePshBtn)
        (Separator                      HistoryTabMenuSep2)
        (PushButton                     HistoryTabMenuPropertiesPshBtn)
    )

    (Resources
        (HistoryTabMenuOpenPshBtn.Label "Open")
        (HistoryTabMenuClosePshBtn.Label "Close")
        (HistoryTabMenuAddToFavPshBtn.Label "Add To Favorites")
        (HistoryTabMenuSep1.TopOffset   0)
        (HistoryTabMenuSep1.BottomOffset 0)
        (HistoryTabMenuSep1.LeftOffset  0)
        (HistoryTabMenuSep1.RightOffset 0)
        (HistoryTabMenuDeletePshBtn.Label "Delete")
        (HistoryTabMenuSep2.TopOffset   0)
        (HistoryTabMenuSep2.BottomOffset 0)
        (HistoryTabMenuSep2.LeftOffset  0)
        (HistoryTabMenuSep2.RightOffset 0)
        (HistoryTabMenuPropertiesPshBtn.Label "Properties")
    )
)

(MenuPane PmObjActMenu
    (Components
        (PushButton                     PmObjActRehilite)
        (PushButton                     PmObjActJntSettings)
        (PushButton                     PmObjActEdit)
        (PushButton                     PmObjActDelete)
        (PushButton                     PmObjActDetails)
        (PushButton                     PmObjActServoMotor)
        (PushButton                     PmObjActForceMotor)
        (PushButton                     PmObjActSpring)
        (PushButton                     PmObjActDamper)
    )

    (Resources
        (PmObjActRehilite.Label         "Rehighlight")
        (PmObjActRehilite.HelpText      "Rehighlight this Entity.")
        (PmObjActRehilite.ActivateCommand "ProCmdMdoRehilite")
        (PmObjActJntSettings.Label      "Joint Settings")
        (PmObjActJntSettings.HelpText   "Set the Joint Axis Setting.")
        (PmObjActJntSettings.ActivateCommand "ProCmdMdoJntSettings")
        (PmObjActEdit.Label             "Edit")
        (PmObjActEdit.HelpText          "Edit this Entity.")
        (PmObjActEdit.ActivateCommand   "ProCmdMdoJntSettings")
        (PmObjActDelete.Label           "Delete")
        (PmObjActDelete.HelpText        "Delete this Entity.")
        (PmObjActDelete.ActivateCommand "ProCmdMdoDelete")
        (PmObjActDetails.Label          "Details")
        (PmObjActDetails.HelpText       "Details of this Entity.")
        (PmObjActDetails.ActivateCommand "ProCmdMdoDetails")
        (PmObjActServoMotor.Label       "Servo Motor")
        (PmObjActServoMotor.HelpText    "Details of this Entity.")
        (PmObjActServoMotor.ActivateCommand "ProCmdMdoServoMotor")
        (PmObjActForceMotor.Label       "Force Motor")
        (PmObjActForceMotor.HelpText    "Details of this Entity.")
        (PmObjActForceMotor.ActivateCommand "ProCmdMdoForceMotor")
        (PmObjActSpring.Label           "Spring")
        (PmObjActSpring.HelpText        "Details of this Entity.")
        (PmObjActSpring.ActivateCommand "ProCmdMdoSpring")
        (PmObjActDamper.Label           "Damper")
        (PmObjActDamper.HelpText        "Details of this Entity.")
        (PmObjActDamper.ActivateCommand "ProCmdMdoDamper")
    )
)

(MenuPane sstbar_popup
    (Components
        (PushButton                     buffer_clean)
    )

    (Resources
        (buffer_clean.Label             "Clear")
    )
)
