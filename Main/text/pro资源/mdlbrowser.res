!
!  30-APR-97 H-03-08 DLT  $$1 Created.
!  13-MAY-97 H-03-10 DLT  $$2 Modified size of the window.
!  15-MAY-97 H-03-10 DLT  $$3 Added preview.
!  03-JUN-97 H-03-13 DLT  $$4 Added columns.
!  11-JUN-97 H-03-14 DLT  $$5 Added sorting and editing.
!  24-JUL-97 H-03-17 DLT  $$6 hide input tab
!  25-JUL-97 H-03-17 GSH  $$7 told the dialog to apppear in the center.
!  11-JUN-97 H-03-17 DLT  $$8 Added delete and copy.
!  21-AUG-97 H-03-18 GSH  $$9 made it forget its previous location
!  20-NOV-97 H-03-31 HMT  $$10 Selection policy=2 for AssyTree
!  01-DEC-97 H-03-31 HMT       AssyTree.BackgroundColor = 15
!  19-DEC-97 H-03-34 MCA  $$11 Specified OK to be default button.
!  25-May-98 I-01-09 dan  $$12 TreeNodeType[s] "current" and no_icon for verify.
!  10-Aug-98 I-01-16 XKA  $$13 Changed BackgroundColor from White(15) to
!			       SunkenBackground(22).
!  14-Sep-98 I-01-18 dan  $$14 added one line help messages (button.HelpText) 
!                              for search ( find ) and ShowTable buttons.
!  17-Sep-98 I-01-19 dan  $$15 Removed(commented) 'FileMenu' from MBar, since 
!                       it is currently contains nothing, excpet exit which is
!                              same as cancel, and hence reduntant.
!  27-Jun-99 I-03-11 lea  $$16 Fixed width of dialog.
!  25-Nov-99 I-03-22 VUD  $$17 Added new Search possibilities.
!  19-Aug-01 J-03-06 mkup $$18 Added unverified instances selection.
!  20-Sep-01 J-03-08 mkup $$19 Removed SearchMenu.
!

(Dialog mdlbrowser
    (Components
        (MenuBar                        MBar
!                                       FileMenu
                                        TreeMenu
                                        ActMenu)
!                                        SearchMenu)
        (Tree                           AssyTree)
        (PushButton                     OK)
        (PushButton                     Cancel)
        (Tab                            edit_tab
                                        NoEdit
                                        ComboEdit)
    )

    (Resources
        (AssyTree.SelectionPolicy       2)
        (AssyTree.Rows                  11)
        (AssyTree.Columns               17)
        (AssyTree.MinColumns            16)
        (AssyTree.TreeLinkStyle         95)
        (AssyTree.TreeChildIndent       2)
        (AssyTree.TreeAttributeWindow   True)
        (AssyTree.PopupMenu             "ActMenu")
        (AssyTree.BackgroundColor       22)
        (AssyTree.SelectByCell          True)
        (AssyTree.TreeNodeTypeNames "current"
                                    "no_icon" )
        (AssyTree.TreeNodeTypeHelpTexts "current"
                                        "no_icon")
        (AssyTree.TreeNodeTypePrefixs ""
                                      "") 
        (AssyTree.TreeNodeTypeAppends ""
                                      "")
        (AssyTree.TreeNodeTypeExpandedImages "arrow_sc"
                                             "")
        (AssyTree.TreeNodeTypeCollapsedImages "arrow_sc"
                                              "")
        (OK.Label                       "  OK  ")
        (OK.HelpText                    "Select highlighted model.")
        (OK.TopOffset                   2)
        (OK.BottomOffset                2)
        (Cancel.Label                   "Cancel")
        (Cancel.HelpText                "Abort the selection.")
        (Cancel.TopOffset               2)
        (Cancel.BottomOffset            2)
        (edit_tab.Collapsable           True)
        (.Label                         "Family Tree")
        (.HelpTimer                     10)
        (.StartLocation                 5)
        (.TopOffset                     1)
        (.BottomOffset                  1)
        (.LeftOffset                    1)
        (.RightOffset                   1)
        (.RememberPosition              False)
        (.DefaultButton                 "OK")
        (.Layout
            (Grid (Rows 0 1 0) (Cols 1)
                edit_tab
                AssyTree
                (Grid (Rows 0) (Cols 1 1 1 1 1)
                    (Pos 1 2)
                    OK
                    (Pos 1 4)
                    Cancel
                )
            )
        )
    )
)

! (MenuPane FileMenu
!     (Components
!         (PushButton                     Exit)
!     )
! 
!     (Resources
!         (Exit.Label                     "Quit")
!         (Exit.HelpText                  "Cancel selection and close the browser.")
!         (.Label                         "File")
!     )
! )

(MenuPane TreeMenu
    (Components
        (CascadeButton                  ExpCascade
                                        ExpandMenu)
        (CascadeButton                  ClpCascade
                                        ClpMenu)
        (Separator                      Sep1)
        (PushButton                     Find)
        (CascadeButton                  Sort
                                        SortMenu)
        (CascadeButton                  ClmCascade
                                        ColumnMenu)
    )

    (Resources
        (ExpCascade.Label               "Expand")
        (ClpCascade.Label               "Collapse")
        (Sep1.TopOffset                 0)
        (Sep1.BottomOffset              0)
        (Sep1.LeftOffset                0)
        (Sep1.RightOffset               0)
        (Find.Label                     "Search...")
        (Find.HelpText                  "Search by expression.")
        (Sort.Label                     "Sort")
        (ClmCascade.Label               "Columns")
        (.Label                         "Tree")
    )
)

(MenuPane ExpandMenu
    (Components
        (PushButton                     ExpandAll)
        (PushButton                     ExpandBranch)
        (PushButton                     Expand)
    )

    (Resources
        (ExpandAll.Label                "All")
        (ExpandAll.HelpText             "Expand all the models.")
        (ExpandBranch.Label             "Branch")
        (ExpandBranch.HelpText          "Expand selected model and its submodels.")
        (Expand.Label                   "One")
        (Expand.HelpText                "Expand selected model only.")
        (.Label                         "Expand")
    )
)

(MenuPane ClpMenu
    (Components
        (PushButton                     CollapseAll)
        (PushButton                     CollapseBranch)
        (PushButton                     Collapse)
    )

    (Resources
        (CollapseAll.Label              "All")
        (CollapseAll.HelpText           "Collapse all the models.")
        (CollapseBranch.Label           "Branch")
        (CollapseBranch.HelpText        "Collapse selected model and its submodels.")
        (Collapse.Label                 "One")
        (Collapse.HelpText              "Collapse selected model only.")
        (.Label                         "Collapse")
    )
)

(MenuPane SortMenu
    (Components
        (CheckButton                    SortName)
        (CheckButton                    SortClmn)
    )

    (Resources
        (SortName.Label                 "By Name")
        (SortName.HelpText              "Sort content by model name")
        (SortClmn.Label                 "By Columns")
        (SortClmn.HelpText              "Sort content by columns")
        (.Label                         "Sort")
    )
)

(MenuPane ColumnMenu
    (Components
        (PushButton                     SwitchCol)
        (PushButton                     Format)
    )

    (Resources
        (SwitchCol.Label                "Add/Remove...")
        (SwitchCol.HelpText             "Select which info columns should be displayed")
        (Format.Label                   "Format...")
        (Format.HelpText                "Reconfigure widths and positions of currently displayed columns.")
        (.Label                         "Columns")
    )
)

(MenuPane ActMenu
    (Components
        (PushButton                     Preview)
        (PushButton                     ShowTable)
        (PushButton                     HideTable)
        (PushButton                     ShowTags)
        (PushButton                     AddInst)
        (PushButton                     DelInst)
        (PushButton                     CopyInst)
        (PushButton                     AddItem)
        (PushButton                     DelItem)
        (PushButton                     ClosePreview)
        (Separator                      Sep2)
        (PushButton                     SelectAllInst)
        (PushButton                     UnselectAllInst)
        (Separator                      Sep3)
        (PushButton                     SelectVerified)
        (PushButton                     SelectUnverified)
        (PushButton                     SelectFailed)
    )

    (Resources
        (Preview.Label                  "Preview")
        (Preview.HelpText               "Display selected model in a window.")
        (ShowTable.Label                "Show Table")
        (ShowTable.HelpText             "Show contents of driving Table")
        (HideTable.Label                "Hide Table")
        (HideTable.HelpText             "Hide contents of driving Table")
        (ShowTags.Label                 "ShowTags")
        (AddInst.Label                  "Add Instance")
        (DelInst.Label                  "Delete")
        (CopyInst.Label                 "Copy")
        (AddItem.Label                  "Add Item")
        (DelItem.Label                  "Delete Item")
        (ClosePreview.Visible           False)
        (Sep2.TopOffset                 0)
        (Sep2.BottomOffset              0)
        (Sep2.LeftOffset                0)
        (Sep2.RightOffset               0)
        (SelectAllInst.Label            "Select All")
        (SelectAllInst.HelpText         "Select all the instances in the table.")
        (UnselectAllInst.Label          "Unselect All")
        (UnselectAllInst.HelpText       "Unselect all the instances in the table.")
        (Sep3.TopOffset                 0)
        (Sep3.BottomOffset              0)
        (Sep3.LeftOffset                0)
        (Sep3.RightOffset               0)
        (SelectVerified.Label           "Select Verified")
        (SelectVerified.HelpText        "Select all the instances with successfull verify status.")
        (SelectUnverified.Label         "Select Unverified")
        (SelectUnverified.HelpText      "Select all the unverified instances.")
        (SelectFailed.Label             "Select Failed")
        (SelectFailed.HelpText          "Select all the instances with Failed verify status.")
        (.Label                         "Edit")
    )
)

!(MenuPane SearchMenu
!    (Components
!        (PushButton                     SearchByItem)
!        (PushButton                     SearchByName)
!        (PushButton                     SearchByExpression)
!    )
!
!    (Resources
!        (SearchByItem.Label             "by Item")
!        (SearchByName.Label             "by Name")
!        (SearchByExpression.Label       "by Expression")
!        (.Label                         "Search")
!    )
!)

(Layout NoEdit
    (Components
        (Separator                      Separator5)
    )

    (Resources
        (Separator5.TopOffset           0)
        (Separator5.BottomOffset        0)
        (Separator5.LeftOffset          0)
        (Separator5.RightOffset         0)
        (.Layout
            (Grid (Rows 1) (Cols 1)
                Separator5
            )
        )
    )
)

(Layout ComboEdit
    (Components
        (OptionMenu                     EditPanel)
    )

    (Resources
        (EditPanel.Columns              5)
        (EditPanel.Editable             True)
        (.Label                         "ComboEdit")
        (.Layout
            (Grid (Rows 1) (Cols 1)
                EditPanel
            )
        )
    )
)
