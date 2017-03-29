!
!  19-Jul-99 I-03-11 zjl/hanoch    $$1   Submitted
!  02-NOV-99 I-03-20 SHK $$2 Added image pkgchld for child of packaged comps.
!  22-Feb-00 J-01-03 jas $$3 Added support for embedded editing components
!  11-Apr-01 J-01-32 jas $$4 Added default button support
!  03-Jun-01 J-03-01 Idan     Created.	
!  04-Jul-01 J-03-03 Moti $$5    Added ScrollBarsWhenNeeded . 
!                               removed Layouts labels .
!  22-Jul-01 J-03-04 Moti     Replaced history tab icon , removed 
!                             drag & drop ability .  
!  20-Aug-01 J-03-06 Moti     Replaced old Favorites GUI with new one .
!  26-Aug-01 J-03-07 Moti   Added functionality to favorites .
!  05-SEP-01 J-03-07 Hanoch $$6   Rename from mdltree_n_url_ph
!  14-Oct-01 J-03-10 Moti   Changed TabTree style .
!  28-Oct-01 J-03-11 AlexM  Add SubToolBars Fav_tb and history_tb.
!  04-NOV-01 J-03-12 Hanoch  Added help tags
!  05-Nov-01 J-03-13 Moti   Removed columns from tab .
!  06-Dec-01 J-03-14 Moti $$7 Moved file pro_explorer_ph.res to here .
!  25-Nov-01 J-03-14 elevy  $$8 Remove embedded components.
!  27-Dec-01 J-03-15 Peter $$9  Removed mnemonics for Fav_tb.
!  15-Jan-02 J-03-17 Moti $$10  Added Clear History button to History tab .
!  17-Jan-02 J-03-17 RSS  $$11  Added Model tree cascade buttons.
!  30-Jan-02 J-03-18 Moti $$12  Added two extra node types .
!  31-Jan-02 J-03-18 RSS  $$13  Removed MdltreeNameLayout. Modified name of 
!                               CascadeButton2 and CascadeButton3
!  02-Feb-02 J-03-18+ Moti $$14 Fully removed histTitleLay
!  11-Feb-02 J-03-19  RSS  $$15 Modified unsel image for HighlightChkBtn.
!                               Removed FindPb. Modified Helptext for FilterPb.
!  05-Feb-02 J-03-19 AlexM $$16 Fix names separators
!  19-Mar-02 J-03-21 RSS   $$17 Reverted back to old commands.
!  12-Mar-02 J-03-21 AlexM $$18 Add  command ProCmdExplorerTabClrHis
!  31-Mar-02 J-03-22 Moti  $$19 Changes "Close history" button position .
!  10-Apr-02 J-03-23 aap   $$20 LayerTreePb is a CheckButton
!  21-Apr-02 J-03-24 Moti  $$21 Changed History and Favorites buttons resize .
!  06-May-02 J-03-25 Moti  $$22 Changed Tree style to work as a single click
!  16-May-02 J-03-26 Moti  $$23 Added open_folder node type .
!  22-MAY-02 J-03-26 Hanoch     Added two help tags
! 11-Jun-02  J-03-27  VJ   $$24 Moved View->Model Tree Setup->Use Model in Tree
!                               to "Show" option in the Model tree toolbar.
!                               (Refer SPR 942969)
! 22-Jul-02  J-03-30 IKB   $$25 Added one command ProCmdMdlTreeShowLyrs 
!                               to fix Bug#959645
! 12-Sep-02  J-03-34 RSS   $$26 Moved LayerTreePB to top in MyShowMenuPane.
! 22-Oct-02  J-03-36 SMM   $$27 Fixed SPR 976855 - mnemonics
! 22-Nov-02  J-03-38 Hila  $$28 Added cascade button Piping_View.
! 16-Apr-03  K-01-05 RSS   $$29 Added Symbol names for defining and failed.
! 14-May-03  K-01-07 RSS   $$30 Added Symbol names for pasting.
! 25-May-03  K-01-08 Moti  $$31 changed url_icon to 3party_browser_top .
! 08-Sep-03  K-01-14 Moti  $$32 Changed label in ClearHistoryPB.
! 19-Nov-03  K-01-19 RSS   $$33 Added Symbol names for joint_placed.
! 29-Feb-04  K-01-24 Moti  $$34 Added nemonic to Clear History.

(Layout TreeLayout
    (Components
        (Tab                            ProExplorerTab
                                        MdlTreeLay
                                        FavLay
                                        HistLay)
    )

    (Resources
        (ProExplorerTab.Decorated       True)
        (ProExplorerTab.FixedWidth      True)
        (ProExplorerTab.ValidDropSite   False)
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                ProExplorerTab
            )
        )
    )
)

(Layout MdlTreeLay
    (Components
        (Tree                           AssyTree)
        (SubLayout                      Layout1)
    )

    (Resources
        (AssyTree.SelectionPolicy       4)
        (AssyTree.Rows                  28)
        (AssyTree.Columns               5)
        (AssyTree.TreeLinkStyle         95)
        (AssyTree.TreeChildIndent       2)
        (AssyTree.TreeAttributeWindow   True)
        (AssyTree.TreeDragEnabled       True)
        (AssyTree.PopupMenu             "ActionMenu")
        (AssyTree.SelectByCell          True)
        (AssyTree.SymbolNames           "sup"
                                        "pack"
                                        "pkgchld"
                                        "incomp"
					"defining"
                                        "failed"
                                        "pasting"
                                        "joint_placed")
        (AssyTree.SymbolImages          "sup"
                                        "pack"
                                        "pkgchld"
                                        "incomp"
					"feat_create"
                                        "feat_fail"
                                        "feat_paste"
                                        "joint_placed")
        (AssyTree.ActivateOnReturn      False)
        (.Bitmap                        "tree_image")
        (.HelpText                      "Model Tree")
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.HelpTag                       "main_wind_dlg@ModelTree")
        (.Layout
            (Grid (Rows 0 1) (Cols 1)
                Layout1
                AssyTree
            )
        )
    )
)


(Layout Layout1
    (Components
        (CascadeButton                  ShowCB
                                        MyShowMenuPane)
        (CascadeButton                  Piping_View
                                        Piping_Menu)
        (CascadeButton                  SettingsCB
                                        SettingsMenuPane)
    )

    (Resources
        (ShowCB.Label                   "Sh&ow")
        (ShowCB.AttachLeft              True)
        (Piping_View.Label              "&View")
        (Piping_View.HelpText           "Show Piping system tree views")
        (Piping_View.Visible            False)
        (SettingsCB.Label               "Settin&gs")
        (SettingsCB.AttachLeft          True)
        (.AttachLeft                    True)
        (.Layout
            (Grid (Rows 1) (Cols 1 1 1)
                ShowCB
	        Piping_View
                SettingsCB
            )
        )
    )
)

(MenuPane MyShowMenuPane
    (Components
        (CheckButton                    LayerTreePB)
        (Separator                      MyShowMenuPane_Sep2)
        (PushButton                     ExpandAllPb)
        (PushButton                     CollapseAllPb)
        (Separator                      MyShowMenuPane_Sep1)
        (CheckButton                    HighlightChkBtn)
        (Separator                      MyShowMenuPane_Sep3)
        (PushButton                     UseModelOfViewPb)
    )

    (Resources
        (LayerTreePB.Label              "&Layer Tree")
        (LayerTreePB.HelpText           "Set layers, layer items and display states")
        (LayerTreePB.ActivateCommand    "ProCmdMdlTreeShowLyrs")
        (MyShowMenuPane_Sep2.TopOffset  0)
        (MyShowMenuPane_Sep2.BottomOffset 0)
        (MyShowMenuPane_Sep2.LeftOffset 0)
        (MyShowMenuPane_Sep2.RightOffset 0)
        (ExpandAllPb.Bitmap             "tree_expand_all.bif")
        (ExpandAllPb.Label              "&Expand  All")
        (ExpandAllPb.HelpText           "Expand all branches")
        (ExpandAllPb.ActivateCommand    "ProCmdMdlTreeExpandAll")
        (CollapseAllPb.Bitmap           "tree_collapse_all.bif")
        (CollapseAllPb.Label            "&Collapse All")
        (CollapseAllPb.HelpText         "Collapse all branches")
        (CollapseAllPb.ActivateCommand  "ProCmdMdlTreeCollapseAll")
        (MyShowMenuPane_Sep1.TopOffset  0)
        (MyShowMenuPane_Sep1.BottomOffset 0)
        (MyShowMenuPane_Sep1.LeftOffset 0)
        (MyShowMenuPane_Sep1.RightOffset 0)
        (HighlightChkBtn.Label          "&Highlight Model")
        (HighlightChkBtn.HelpText       "Selected model tree items will/will not highlight in graphics area")
        (HighlightChkBtn.Selected_image "UI Menu 3State sel image")
        (HighlightChkBtn.Unselected_image "UI Menu 3State unsel image")
        (HighlightChkBtn.ActivateCommand "ProCmdMdlTreeHighlight")
        (MyShowMenuPane_Sep3.TopOffset  0)
        (MyShowMenuPane_Sep3.BottomOffset 0)
        (MyShowMenuPane_Sep3.LeftOffset 0)
        (MyShowMenuPane_Sep3.RightOffset 0)
        (UseModelOfViewPb.Label              "Use Model in Tree")
        (UseModelOfViewPb.HelpText           "Selected drawing view will be displayed in the model tree.")
        (UseModelOfViewPb.ActivateCommand    "ProCmdMdlTreeSwitchView")
    )
)

(MenuPane Piping_Menu
    (Components
        (CheckButton                    PCompView)
        (CheckButton                    PSpoolView)
        (CheckButton                    PipeActive)
    )

    (Resources
        (PCompView.Label                "Pipe&line View")
        (PCompView.HelpText             "Show the components of the pipelines")
        (PCompView.ActivateCommand      "ProCmdMdlTreePCompView")
        (PSpoolView.Label               "&Spool View")
        (PSpoolView.HelpText            "Show the pipe spools of the pipelines")
        (PSpoolView.ActivateCommand     "ProCmdMdlTreePSpoolView")
        (PipeActive.Label               "&From Active Asm")
        (PipeActive.HelpText            "Show the piping system tree from Active assembly")
        (PipeActive.ActivateCommand     "ProCmdMdlTreePipeActive")
        (.Label                         "&View")
    )
)

(MenuPane SettingsMenuPane
    (Components
        (PushButton                     FilterPb)
        (PushButton                     ColumbPb)
        (Separator                      SettingsMenuPane_Sep1)
        (PushButton                     LoadCfgPb)
        (PushButton                     SaveCfgPb)
        (PushButton                     WinCfgPb)
        (Separator                      SettingsMenuPane_Sep2)
        (PushButton                     SaveAsTxtPb)
    )

    (Resources
        (FilterPb.Bitmap                "tree_filter.bif")
        (FilterPb.Label                 "Tree &Filters...")
        (FilterPb.HelpText              "Control display of model tree items by type and status")
        (FilterPb.ActivateCommand       "ProCmdMdlTreeFilter")
        (ColumbPb.Bitmap                "tree_column.bif")
        (ColumbPb.Label                 "Tree &Columns...")
        (ColumbPb.HelpText              "Model tree column display options")
        (ColumbPb.ActivateCommand       "ProCmdMdlTreeColumns")
        (SettingsMenuPane_Sep1.TopOffset 0)
        (SettingsMenuPane_Sep1.BottomOffset 0)
        (SettingsMenuPane_Sep1.LeftOffset 0)
        (SettingsMenuPane_Sep1.RightOffset 0)
        (LoadCfgPb.Bitmap               "open.bif")
        (LoadCfgPb.Label                "&Open Settings File...")
        (LoadCfgPb.HelpText             "Load previously stored settings from file")
        (LoadCfgPb.ActivateCommand      "ProCmdMdlTreeRetrieve")
        (SaveCfgPb.Bitmap               "save.bif")
        (SaveCfgPb.Label                "&Save Settings File...")
        (SaveCfgPb.HelpText             "Store current settings (info columns, etc.) on disk.")
        (SaveCfgPb.ActivateCommand      "ProCmdMdlTreeSave")
        (WinCfgPb.Label                 "&Apply Settings from Window..")
        (WinCfgPb.HelpText              "Apply settings of another window")
        (WinCfgPb.ActivateCommand       "ProCmdMdlTreeApplyCfg")
        (SettingsMenuPane_Sep2.TopOffset 0)
        (SettingsMenuPane_Sep2.BottomOffset 0)
        (SettingsMenuPane_Sep2.LeftOffset 0)
        (SettingsMenuPane_Sep2.RightOffset 0)
        (SaveAsTxtPb.Label              "Save &Model Tree...")
        (SaveAsTxtPb.HelpText           "Store displayed model tree information on disk in text format")
        (SaveAsTxtPb.ActivateCommand    "ProCmdMdlTreeSaveAsText")
    )
)

(Layout FavLay
    (Components
        (SubLayout                      contrLay)
        (SubLayout                      Layout2)
        (PushButton                     DummyPshBtn)
    )

    (Resources
        (DummyPshBtn.Visible            False)
        (DummyPshBtn.AttachRight        True)
        (.Bitmap                        "wildfire_favorites")
        (.HelpText                      "Favorites")
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.HelpTag                       "main_wind_dlg@Favorites")
        (.Layout
            (Grid (Rows 0 1) (Cols 1)
                (Grid (Rows 0) (Cols 0 1)
                    contrLay
                    DummyPshBtn
                )
                Layout2
            )
        )
    )
)

(Layout contrLay
    (Components
        (SubToolBar                     Fav_tb)
    )

    (Resources
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                Fav_tb
            )
        )
    )
)

(ToolBar Fav_tb
    (Components
        (PushButton                     AddFavPshBtn)
        (PushButton                     OrgFavPshBtn)
    )

    (Resources
        (AddFavPshBtn.Bitmap            "add_fold_sel.bif")
        (AddFavPshBtn.Label             "Add...")
        (AddFavPshBtn.Sensitive         False)
        (AddFavPshBtn.ButtonStyle       3)
        (AddFavPshBtn.AttachLeft        True)
        (AddFavPshBtn.AttachTop         True)
        (AddFavPshBtn.AttachBottom      True)
        (AddFavPshBtn.HelpTag           "main_wind_dlg@FavoritAdd")
        (OrgFavPshBtn.Bitmap            "org_fold_sel.bif")
        (OrgFavPshBtn.Label             "Organize...")
        (OrgFavPshBtn.ButtonStyle       3)
        (OrgFavPshBtn.AttachLeft        True)
        (OrgFavPshBtn.AttachTop         True)
        (OrgFavPshBtn.AttachBottom      True)
        (OrgFavPshBtn.HelpTag           "main_wind_dlg@FavoritOrganize")
        (.FixedWidth                    True)
    )
)

(Layout Layout2
    (Components
        (Tree                           TabTree)
    )

    (Resources
        (TabTree.SelectionPolicy        2)
        (TabTree.Columns                5)
        (TabTree.TreeLinkStyle          95)
        (TabTree.TreeNodeTypeNames      "file"
                                        "folder"
                                        "proep_folder"
                                        "config_folder"
                                        "open_folder")
        (TabTree.TreeNodeTypeHelpTexts  ""
                                        ""
                                        ""
                                        ""
                                        "")
        (TabTree.TreeNodeTypePrefixs    ""
                                        ""
                                        ""
                                        ""
                                        "")
        (TabTree.TreeNodeTypeAppends    ""
                                        ""
                                        ""
                                        ""
                                        "")
        (TabTree.TreeNodeTypeExpandedImages "bookmark.bif"
                                        "closed_folder.bif"
                                        "bookmark.bif"
                                        "bookmark.bif"
                                        "openfolder.bif")
        (TabTree.TreeNodeTypeCollapsedImages "bookmark.bif"
                                        "closed_folder.bif"
                                        "bookmark.bif"
                                        "bookmark.bif"
                                        "openfolder.bif")
        (TabTree.TreeDragEnabled        True)
        (TabTree.PopupMenu              "FavTabMenuPane")
        (TabTree.SelectCBRegardless     1)
        (TabTree.ValidDropSite          False)
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                TabTree
            )
        )
    )
)


(Layout HistLay
    (Components
        (List                           listHist)
        (SubLayout                      historyControlsLay)
        (PushButton                     closeHistoryPB)
    )

    (Resources
        (listHist.SelectionPolicy       2)
        (listHist.Columns               5)
        (listHist.Decorated             False)
        (listHist.ListType              True)
        (listHist.PopupMenu             "HistoryTabMenuPane")
        (listHist.SymbolNames           "date_folder"
                                        "site_folder"
                                        "url_icon"
                                        "empty_icon")
        (listHist.SymbolImages          "date_folder.bif"
                                        "site_folder.bif"
                                        "3party_browser_top.bif"
                                        "empty_icon.bif")
        (closeHistoryPB.Bitmap          "UI close image")
        (closeHistoryPB.ButtonStyle     3)
        (closeHistoryPB.AttachRight     True)
        (closeHistoryPB.AttachTop       True)
        (.Bitmap                        "embrws_tbar_history")
        (.HelpText                      "History")
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.HelpTag                       "main_wind_dlg@History")
        (.Layout
            (Grid (Rows 0 1) (Cols 1)
                (Grid (Rows 0) (Cols 0 1)
                    historyControlsLay
                    closeHistoryPB
                )
                listHist
            )
        )
    )
)

(Layout historyControlsLay
    (Components
        (SubToolBar                     history_tb)
    )

    (Resources
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                history_tb
            )
        )
    )
)

(ToolBar history_tb
    (Components
        (CascadeButton                  HistViewBy
                                        HistViewMenu)
        (PushButton                     HistSearchPB)
        (PushButton                     ClearHistoryPB)
    )

    (Resources
        (HistViewBy.Label               "View")
        (HistViewBy.ButtonStyle         3)
        (HistViewBy.AttachLeft          True)
        (HistViewBy.LeftOffset          2)
        (HistSearchPB.Bitmap            "search.bif")
        (HistSearchPB.Label             "Search")
        (HistSearchPB.Visible           False)
        (HistSearchPB.ButtonStyle       3)
        (HistSearchPB.AttachLeft        True)
        (ClearHistoryPB.Bitmap          "delete_history.bif")
        (ClearHistoryPB.Label           "Clear &History")
        (ClearHistoryPB.HelpText        "Clear History")
        (ClearHistoryPB.ButtonStyle     3)
        (ClearHistoryPB.AttachLeft      True)
        (ClearHistoryPB.LeftOffset      2)
        (ClearHistoryPB.ActivateCommand "ProCmdExplorerTabClrHis")
        (.FixedWidth                    True)
    )
)

(MenuPane HistViewMenu
    (Components
        (RadioGroup                     HistViewOptRG)
    )

    (Resources
        (HistViewOptRG.Names            "date"
                                        "site"
                                        "visited"
                                        "today")
        (HistViewOptRG.Labels           "By Date"
                                        "By Site"
                                        "By Most Visited"
                                        "By Order Visited Today")
    )
)
