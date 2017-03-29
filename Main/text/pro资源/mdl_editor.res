!  May-25-97  H-03-12  RLD   $$1  Created
! 07-Jul-97 H-03-16 hmt      $$2 Use select icon
! 24-Jan-02 J-03-18 AW       $$3 Fixed syntax

(Dialog mdl_editor
    (Components
        (Label                          cfglabel)
        (InputPanel                     cfgname)
        (Label                          treedispLab)
        (RadioGroup                     treedisp)
        (Separator                      Separator1)
        (SubLayout                      Layout1)
        (PushButton                     select)
        (PushButton                     undolast)
        (PushButton                     byrule)
        (PushButton                     byrep)
        (PushButton                     fromto)
        (PushButton                     selall)
        (SubLayout                      appl)
        (PHolder                        appl)
    )

    (Resources
        (cfglabel.AttachLeft                  True)
        (cfglabel.TopOffset                   8)
        (cfglabel.BottomOffset                10)
        (cfglabel.LeftOffset                  4)
        (cfglabel.RightOffset                 5)
        (cfglabel.Label                       "Display Name")
        (cfgname.TopOffset          8)
        (cfgname.BottomOffset       10)
        (cfgname.LeftOffset         0)
        (cfgname.RightOffset        4)
        (cfgname.Value              "vis0005")
        (cfgname.Columns            10)
        (treedispLab.AttachLeft            True)
        (treedispLab.TopOffset             0)
        (treedispLab.BottomOffset          0)
        (treedispLab.LeftOffset            4)
        (treedispLab.RightOffset           0)
        (treedispLab.Label                 "Components Displayed in Tree")
        (Separator1.TopOffset           5)
        (Separator1.BottomOffset        5)
        (Separator1.LeftOffset          0)
        (Separator1.RightOffset         0)
        (select.TopOffset               2)
        (select.BottomOffset            0)
        (select.LeftOffset              20)
        (select.RightOffset             0)
        (select.Bitmap             "sel_pick")
        (undolast.TopOffset             0)
        (undolast.BottomOffset          0)
        (undolast.LeftOffset            2)
        (undolast.RightOffset           2)
        (undolast.Label                 "Undo Last")
        (treedisp.AttachBottom      False)
        (treedisp.TopOffset         0)
        (treedisp.BottomOffset      4)
        (treedisp.LeftOffset        20)
        (treedisp.RightOffset       4)
        (treedisp.Names             "all"
                                        "marked")
        (treedisp.Labels            "All Components"
                                        "Marked Only")
        (byrule.TopOffset               2)
        (byrule.BottomOffset            4)
        (byrule.LeftOffset              20)
        (byrule.RightOffset             0)
        (byrule.Label                   "By Rule...")
        (byrep.TopOffset                2)
        (byrep.BottomOffset             4)
        (byrep.LeftOffset               2)
        (byrep.RightOffset              2)
        (byrep.Label                    "By Rep...")
        (fromto.TopOffset               2)
        (fromto.BottomOffset            0)
        (fromto.LeftOffset              2)
        (fromto.RightOffset             20)
        (fromto.Label                   "Range...")
        (selall.TopOffset               2)
        (selall.BottomOffset            4)
        (selall.LeftOffset              2)
        (selall.RightOffset             20)
        (selall.Label                   "All")
        (.Layout
            (Grid (Rows 1 0 1 0 1 1 0 1) (Cols 1)
                (Grid (Rows 1) (Cols 0 1)
                    cfglabel
                    cfgname
                )
                treedispLab
                treedisp
                appl
                (Grid (Rows 1 0) (Cols 1 1 1)
                    select
                    undolast
                    fromto
                    byrule
                    byrep
                    selall
                )
                Separator1
                Layout1
            )
        )
    )
)

(Layout Layout1
    (Components
        (PushButton                     accept)
        (PushButton                     preview)
        (PushButton                     Cancel)
    )

    (Resources
        (accept.TopOffset               0)
        (accept.BottomOffset            0)
        (accept.LeftOffset              0)
        (accept.RightOffset             0)
        (accept.Label                   "OK")
        (preview.TopOffset                0)
        (preview.BottomOffset             0)
        (preview.LeftOffset               2)
        (preview.RightOffset              0)
        (preview.Label                    "Preview")
        (Cancel.TopOffset               0)
        (Cancel.BottomOffset            0)
        (Cancel.LeftOffset              2)
        (Cancel.RightOffset             0)
        (Cancel.Label                   "Cancel")
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.TopOffset                     4)
        (.BottomOffset                  4)
        (.LeftOffset                    4)
        (.RightOffset                   4)
        (.Layout
            (Grid (Rows 1) (Cols 1 1 1)
                accept
                preview
                Cancel
            )
        )
    )
)
