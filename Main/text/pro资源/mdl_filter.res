!
! 20-MAY-97 H-03-11 GSH  $$1  Created.
! 22-MAY-97 H-03-12 HMT  $$2  Corrected appearance
! 20-OCT-97 H-03-27 HMT  $$3  Icons for 'sell all' and 'unsel all'
! 06-JAN-98 H-03-36 HMT  $$4  Added helptext for icons
! 15-Sep-98 I-01-20 SCL  $$5  Set .KeyboardInput for tool-style buttons
! 02-DEC-98 I-01-27 JLM  $$6  Removed .Resizable, so now its set to True
! 30-Sep-99 I-03-17 zjl  $$7  added node type status filter buttons.
! 02-Nov-99 I-03-20 PMU  $$8  fixed cosmetics and layout
! 12-Mar-00 J-01-03 AKam $$9  Added pipe insulation feature.
! 12-May-00 J-01-08 GSB  $$10 Added a tab to cater to hull features.
! 15-May-00 J-01-08 AKam $$11 Added pipe joint feature.
! 04-Mar-01 J-01-27 AKam $$12 Added piping features icons.
! 26-Mar-01 J-01-30 pmu  $$13 Updated piping icons and layout
! 26-Aug-02 J-03-32 RUB  $$14 Removed Hull.
! 20-May-03 K-01-07 SZH  $$15 Added Sketch and Used sketch (from adubey).
! 14-May-03 K-01-07 elevy $$16 Added Sketched curve button
! 14-May-03 K-01-07 SCL  $$17 Renamed Notes to Annotations.
! 22-Sep-03 K-01-16 SZH  $$18 Added used sketch and icon.
!
!        (Separator                      Separator9)
!        (CheckButton                    nonmfg)
!        (nonmfg.Label                   "Non-manufacturing features")
!        (nonmfg.AttachLeft              True)
!        (nonmfg.TopOffset               3)
!        (nonmfg.LeftOffset              5)
!                Separator9
!                nonmfg

(Dialog mdl_filter
    (Components
        (SubLayout                      Layout26)
        (Separator                      Separator5)
        (SubLayout                      Layout1)
    )

    (Resources
        (Separator5.AttachTop           True)
        (Separator5.AttachBottom        True)
        (Separator5.TopOffset           0)
        (Separator5.BottomOffset        0)
        (Separator5.LeftOffset          0)
        (Separator5.RightOffset         0)
        (.Label                         "Model Tree Items")
        (.Resizeable                    False)
        (.DefaultButton                 "ok_btn")
        (.Layout
            (Grid (Rows 1 0 1) (Cols 1)
                Layout26
                Separator5
                Layout1
            )
        )
    )
)

(Layout Layout26
    (Components
        (Tab                            Tab
                                        general_layout
                                        cabling_layout
                                        piping_layout
                                        mfg)
        (SubLayout                      Layout32)
        (SubLayout                      NoteType)
        (Label                          Label1)
        (Separator                      Separator1)
    )

    (Resources
        (Tab.HelpText                   "Toggle display of feature types when features are displayed")
        (Tab.Decorated                  True)
        (Tab.TopOffset                  4)
        (Tab.LeftOffset                 4)
        (Tab.RightOffset                8)
        (Label1.Label                   "Feature Types")
        (Label1.AttachLeft              True)
        (Label1.AttachTop               True)
        (Label1.Alignment               0)
        (Label1.TopOffset               8)
        (Separator1.Orientation         True)
        (Separator1.AttachTop           True)
        (Separator1.AttachBottom        True)
        (Separator1.TopOffset           0)
        (Separator1.BottomOffset        0)
        (Separator1.LeftOffset          0)
        (Separator1.RightOffset         8)
        (.Label                         "By Feature Type")
        (.AttachLeft                    True)
        (.AttachRight                   True)
        (.AttachTop                     True)
        (.AttachBottom                  True)
        (.Layout
            (Grid (Rows 1) (Cols 0 0 1)
                NoteType
                Separator1
                (Grid (Rows 0 1 0) (Cols 1)
                    Label1
                    Tab
                    Layout32
                )
            )
        )
    )
)

(Layout general_layout
    (Components
        (CheckButton                    cosmetic_btn)
        (CheckButton                    csys_btn)
        (CheckButton                    curve_btn)
        (CheckButton                    datum_axis_btn)
        (CheckButton                    datum_btn)
        (CheckButton                    datum_point_btn)
        (CheckButton                    round_btn)
        (CheckButton                    sk_curve_btn)
        (CheckButton                    used_sk_curve_btn)
    )

    (Resources
        (cosmetic_btn.Label             " <cosm>  Cosmetic")
        (cosmetic_btn.AttachLeft        True)
        (cosmetic_btn.AttachTop         True)
        (cosmetic_btn.TopOffset         4)
        (cosmetic_btn.LeftOffset        5)
        (cosmetic_btn.RightOffset       5)
        (cosmetic_btn.SymbolNames       "cosm")
        (cosmetic_btn.SymbolImages      "cosmetic16")
        (csys_btn.Label                 " <csys>  Coordinate System               ")
        (csys_btn.AttachLeft            True)
        (csys_btn.AttachTop             True)
        (csys_btn.TopOffset             4)
        (csys_btn.LeftOffset            5)
        (csys_btn.RightOffset           5)
        (csys_btn.SymbolNames           "csys")
        (csys_btn.SymbolImages          "datum_csys16")
        (curve_btn.Label                " <crv>  Curve")
        (curve_btn.AttachLeft           True)
        (curve_btn.AttachTop            True)
        (curve_btn.TopOffset            4)
        (curve_btn.LeftOffset           5)
        (curve_btn.RightOffset          5)
        (curve_btn.SymbolNames          "crv")
        (curve_btn.SymbolImages         "datum_curve16")
        (datum_axis_btn.Label           " <axis>  Datum Axis")
        (datum_axis_btn.AttachLeft      True)
        (datum_axis_btn.AttachTop       True)
        (datum_axis_btn.TopOffset       4)
        (datum_axis_btn.LeftOffset      5)
        (datum_axis_btn.RightOffset     5)
        (datum_axis_btn.SymbolNames     "axis")
        (datum_axis_btn.SymbolImages    "datum_axis16")
        (datum_btn.Label                " <pln>  Datum Plane")
        (datum_btn.AttachLeft           True)
        (datum_btn.AttachTop            True)
        (datum_btn.TopOffset            4)
        (datum_btn.LeftOffset           5)
        (datum_btn.RightOffset          5)
        (datum_btn.SymbolNames          "pln")
        (datum_btn.SymbolImages         "datum_plane16")
        (datum_point_btn.Label          " <pnt>  Datum Point")
        (datum_point_btn.AttachLeft     True)
        (datum_point_btn.AttachTop      True)
        (datum_point_btn.TopOffset      4)
        (datum_point_btn.LeftOffset     5)
        (datum_point_btn.RightOffset    5)
        (datum_point_btn.SymbolNames    "pnt")
        (datum_point_btn.SymbolImages   "datum_point16")
        (round_btn.Label                " <rnd>  Round")
        (round_btn.AttachLeft           True)
        (round_btn.AttachTop            True)
        (round_btn.TopOffset            4)
        (round_btn.LeftOffset           5)
        (round_btn.RightOffset          5)
        (round_btn.SymbolNames          "rnd")
        (round_btn.SymbolImages         "round16")
        (sk_curve_btn.Label             " <sktch>  Sketch")
        (sk_curve_btn.AttachLeft        True)
        (sk_curve_btn.AttachTop         True)
        (sk_curve_btn.TopOffset         4)
        (sk_curve_btn.LeftOffset        5)
        (sk_curve_btn.RightOffset       5)
        (sk_curve_btn.SymbolNames       "sktch")
        (sk_curve_btn.SymbolImages      "sketdtmcrv")
        (used_sk_curve_btn.Label             " <usktch>  Used Sketch")
        (used_sk_curve_btn.AttachLeft        True)
        (used_sk_curve_btn.AttachTop         True)
        (used_sk_curve_btn.TopOffset         4)
        (used_sk_curve_btn.LeftOffset        5)
        (used_sk_curve_btn.RightOffset       5)
        (used_sk_curve_btn.SymbolNames       "usktch")
        (used_sk_curve_btn.SymbolImages      "sketdtmcrv")
        (.Label                         "General")
        (.Layout
            (Grid (Rows 0 0 0 0 0 0 1) (Cols 1 1)
                datum_btn
                sk_curve_btn
                datum_axis_btn
                used_sk_curve_btn
                curve_btn
                (Pos 4 1)
                datum_point_btn
                (Pos 5 1)
                csys_btn
                (Pos 6 1)
                round_btn
                (Pos 7 1)
                cosmetic_btn
            )
        )
    )
)

(Layout cabling_layout
    (Components
        (CheckButton                    cable_btn)
        (CheckButton                    location_btn)
        (CheckButton                    cable_segm_btn)
        (CheckButton                    spool_btn)
        (CheckButton                    cable_cosm_btn)
    )

    (Resources
        (cable_btn.Label                "Cable")
        (cable_btn.AttachLeft           True)
        (cable_btn.AttachTop            True)
        (cable_btn.TopOffset            5)
        (cable_btn.LeftOffset           5)
        (cable_btn.RightOffset          5)
        (location_btn.Label             "Location")
        (location_btn.AttachLeft        True)
        (location_btn.AttachTop         True)
        (location_btn.TopOffset         7)
        (location_btn.LeftOffset        5)
        (location_btn.RightOffset       5)
        (cable_segm_btn.Label           "Cable/Bundle Segment   ")
        (cable_segm_btn.AttachLeft      True)
        (cable_segm_btn.AttachTop       True)
        (cable_segm_btn.TopOffset       7)
        (cable_segm_btn.LeftOffset      5)
        (cable_segm_btn.RightOffset     5)
        (spool_btn.Label                "Spool")
        (spool_btn.AttachLeft           True)
        (spool_btn.AttachTop            True)
        (spool_btn.TopOffset            7)
        (spool_btn.LeftOffset           5)
        (spool_btn.RightOffset          5)
        (cable_cosm_btn.Label           "Cable Cosmetic")
        (cable_cosm_btn.AttachLeft      True)
        (cable_cosm_btn.AttachTop       True)
        (cable_cosm_btn.TopOffset       7)
        (cable_cosm_btn.LeftOffset      5)
        (cable_cosm_btn.RightOffset     5)
        (.Label                         "Cabling")
        (.Layout
            (Grid (Rows 0 0 0 0 1) (Cols 1)
                cable_btn
                location_btn
                cable_segm_btn
                spool_btn
                cable_cosm_btn
            )
        )
    )
)

(Layout piping_layout
    (Components
        (CheckButton                    pipe_line_btn)
        (CheckButton                    line_stock_btn)
        (CheckButton                    pipe_insul_btn)
        (CheckButton                    pipe_branch_btn)
        (CheckButton                    pipe_join_btn)
        (CheckButton                    pipe_ext_btn)
        (CheckButton                    pipe_foll_btn)
        (CheckButton                    pipe_set_start_btn)
        (CheckButton                    pipe_pnt_btn)
        (CheckButton                    pipe_joint_btn)
    )

    (Resources
        (pipe_line_btn.Label            " <pline>  Pipe Line")
        (pipe_line_btn.AttachLeft       True)
        (pipe_line_btn.AttachTop        True)
        (pipe_line_btn.TopOffset        4)
        (pipe_line_btn.LeftOffset       5)
        (pipe_line_btn.RightOffset      0)
        (pipe_line_btn.SymbolNames      "pline")
        (pipe_line_btn.SymbolImages     "pipe_pipeline")
        (line_stock_btn.Label           " <lnstk>  Line Stock")
        (line_stock_btn.AttachLeft      True)
        (line_stock_btn.AttachTop       True)
        (line_stock_btn.TopOffset       4)
        (line_stock_btn.LeftOffset      5)
        (line_stock_btn.RightOffset     0)
        (line_stock_btn.SymbolNames     "lnstk")
        (line_stock_btn.SymbolImages    "pipe_linestock")
        (pipe_insul_btn.Label           " <insul_obj>  Pipe Insulation")
        (pipe_insul_btn.AttachLeft      True)
        (pipe_insul_btn.AttachTop       True)
        (pipe_insul_btn.TopOffset       4)
        (pipe_insul_btn.LeftOffset      5)
        (pipe_insul_btn.RightOffset     0)
        (pipe_insul_btn.SymbolNames     "insul_obj")
        (pipe_insul_btn.SymbolImages    "pipe_insulation")
        (pipe_branch_btn.Label          " <branch>  Pipe Branch")
        (pipe_branch_btn.AttachLeft     True)
        (pipe_branch_btn.AttachTop      True)
        (pipe_branch_btn.TopOffset      4)
        (pipe_branch_btn.LeftOffset     5)
        (pipe_branch_btn.RightOffset    0)
        (pipe_branch_btn.SymbolNames    "branch")
        (pipe_branch_btn.SymbolImages   "pipe_branch")
        (pipe_join_btn.Label            " <connect>  Pipe Connect")
        (pipe_join_btn.AttachLeft       True)
        (pipe_join_btn.AttachTop        True)
        (pipe_join_btn.TopOffset        4)
        (pipe_join_btn.LeftOffset       10)
        (pipe_join_btn.RightOffset      5)
        (pipe_join_btn.SymbolNames      "connect")
        (pipe_join_btn.SymbolImages     "pipe_feature")
        (pipe_ext_btn.Label             " <extend>  Pipe Extend")
        (pipe_ext_btn.AttachLeft        True)
        (pipe_ext_btn.AttachTop         True)
        (pipe_ext_btn.TopOffset         4)
        (pipe_ext_btn.LeftOffset        10)
        (pipe_ext_btn.RightOffset       5)
        (pipe_ext_btn.SymbolNames       "extend")
        (pipe_ext_btn.SymbolImages      "pipe_feature")
        (pipe_foll_btn.Label            " <follow>  Pipe Follow")
        (pipe_foll_btn.AttachLeft       True)
        (pipe_foll_btn.AttachTop        True)
        (pipe_foll_btn.TopOffset        4)
        (pipe_foll_btn.LeftOffset       10)
        (pipe_foll_btn.RightOffset      5)
        (pipe_foll_btn.SymbolNames      "follow")
        (pipe_foll_btn.SymbolImages     "pipe_feature")
        (pipe_set_start_btn.Label       " <sstart>  Pipe Set Start")
        (pipe_set_start_btn.AttachLeft  True)
        (pipe_set_start_btn.AttachTop   True)
        (pipe_set_start_btn.TopOffset   4)
        (pipe_set_start_btn.LeftOffset  10)
        (pipe_set_start_btn.RightOffset 5)
        (pipe_set_start_btn.SymbolNames "sstart")
        (pipe_set_start_btn.SymbolImages "pipe_setstart")
        (pipe_pnt_btn.Label             " <ptopoint>  Pipe To Point")
        (pipe_pnt_btn.AttachLeft        True)
        (pipe_pnt_btn.AttachTop         True)
        (pipe_pnt_btn.TopOffset         4)
        (pipe_pnt_btn.LeftOffset        10)
        (pipe_pnt_btn.RightOffset       5)
        (pipe_pnt_btn.SymbolNames       "ptopoint")
        (pipe_pnt_btn.SymbolImages      "pipe_feature")
        (pipe_joint_btn.Label           " <joint>  Pipe Joint")
        (pipe_joint_btn.AttachLeft      True)
        (pipe_joint_btn.AttachTop       True)
        (pipe_joint_btn.TopOffset       4)
        (pipe_joint_btn.LeftOffset      5)
        (pipe_joint_btn.RightOffset     0)
        (pipe_joint_btn.SymbolNames     "joint")
        (pipe_joint_btn.SymbolImages    "pipe_joint")
        (.Label                         "Piping")
        (.Layout
            (Grid (Rows 0 0 0 0 0) (Cols 1 1)
                pipe_line_btn
                pipe_set_start_btn
                line_stock_btn
                pipe_join_btn
                pipe_insul_btn
                pipe_ext_btn
                pipe_branch_btn
                pipe_foll_btn
                pipe_joint_btn
                pipe_pnt_btn
            )
        )
    )
)

(Layout mfg
    (Components
        (CheckButton                    _invis_oper)
        (CheckButton                    _invis_wkcell)
        (CheckButton                    _invis_seq)
        (CheckButton                    _invis_mfg_geom)
        (CheckButton                    _invis_mat_rmv)
    )

    (Resources
        (_invis_oper.Label              " <oper>  Operation")
        (_invis_oper.AttachLeft         True)
        (_invis_oper.AttachTop          True)
        (_invis_oper.TopOffset          4)
        (_invis_oper.LeftOffset         5)
        (_invis_oper.RightOffset        5)
        (_invis_oper.SymbolNames        "oper")
        (_invis_oper.SymbolImages       "operation")
        (_invis_wkcell.Label            " <wcell>  Workcell")
        (_invis_wkcell.AttachLeft       True)
        (_invis_wkcell.AttachTop        True)
        (_invis_wkcell.TopOffset        4)
        (_invis_wkcell.LeftOffset       5)
        (_invis_wkcell.RightOffset      5)
        (_invis_wkcell.SymbolNames      "wcell")
        (_invis_wkcell.SymbolImages     "wkcell")
        (_invis_seq.Label               " <seq>  Sequence")
        (_invis_seq.AttachLeft          True)
        (_invis_seq.AttachTop           True)
        (_invis_seq.TopOffset           4)
        (_invis_seq.LeftOffset          5)
        (_invis_seq.RightOffset         5)
        (_invis_seq.SymbolNames         "seq")
        (_invis_seq.SymbolImages        "ncseq")
        (_invis_mfg_geom.Label          " <geom>  MFG Geometry")
        (_invis_mfg_geom.AttachLeft     True)
        (_invis_mfg_geom.AttachTop      True)
        (_invis_mfg_geom.TopOffset      4)
        (_invis_mfg_geom.LeftOffset     5)
        (_invis_mfg_geom.RightOffset    5)
        (_invis_mfg_geom.SymbolNames    "geom")
        (_invis_mfg_geom.SymbolImages   "mldvol")
        (_invis_mat_rmv.Label           " <rem>  Material Removal")
        (_invis_mat_rmv.AttachLeft      True)
        (_invis_mat_rmv.AttachTop       True)
        (_invis_mat_rmv.TopOffset       4)
        (_invis_mat_rmv.LeftOffset      5)
        (_invis_mat_rmv.RightOffset     5)
        (_invis_mat_rmv.SymbolNames     "rem")
        (_invis_mat_rmv.SymbolImages    "matrmv")
        (.Label                         "MFG")
        (.Layout
            (Grid (Rows 0 0 0 0 1) (Cols 1)
                _invis_oper
                _invis_wkcell
                _invis_seq
                _invis_mfg_geom
                _invis_mat_rmv
            )
        )
    )
)

(Layout Layout32
    (Components
        (PushButton                     select_all_btn)
        (PushButton                     unselect_all_btn)
    )

    (Resources
        (select_all_btn.Bitmap          "selr")
        (select_all_btn.HelpText        "Select all feature types to display in Model Tree")
        (select_all_btn.ButtonStyle     2)
        (select_all_btn.AttachLeft      True)
        (select_all_btn.AttachRight     True)
        (select_all_btn.AttachTop       True)
        (select_all_btn.AttachBottom    True)
        (select_all_btn.KeyboardInput   True)
        (unselect_all_btn.Bitmap        "unselr")
        (unselect_all_btn.HelpText      "Unselect all feature types to display in Model Tree")
        (unselect_all_btn.ButtonStyle   2)
        (unselect_all_btn.AttachLeft    True)
        (unselect_all_btn.AttachRight   True)
        (unselect_all_btn.AttachTop     True)
        (unselect_all_btn.AttachBottom  True)
        (unselect_all_btn.KeyboardInput True)
        (.AttachLeft                    True)
        (.AttachTop                     True)
        (.TopOffset                     2)
        (.BottomOffset                  4)
        (.LeftOffset                    2)
        (.Layout
            (Grid (Rows 1) (Cols 1 1)
                select_all_btn
                unselect_all_btn
            )
        )
    )
)

(Layout NoteType
    (Components
        (CheckButton                    feat_btn)
        (CheckButton                    note_btn)
        (CheckButton                    blank)
        (CheckButton                    mfgowner)
        (CheckButton                    supres_btn)
        (CheckButton                    incomp_btn)
        (CheckButton                    exclud_btn)
        (Label                          Label2)
    )

    (Resources
        (feat_btn.Label                 "Features")
        (feat_btn.HelpText              "Toggle display of features")
        (feat_btn.AttachLeft            True)
        (feat_btn.AttachTop             True)
        (feat_btn.LeftOffset            8)
        (note_btn.Label                 "Annotations")
        (note_btn.HelpText              "Toggle display of annotations")
        (note_btn.AttachLeft            True)
        (note_btn.AttachTop             True)
        (note_btn.LeftOffset            8)
        (blank.Label                    "Blanked Objects")
        (blank.HelpText                 "Toggle display of blanked mold / cast components")
        (blank.AttachLeft               True)
        (blank.AttachTop                True)
        (blank.LeftOffset               8)
        (mfgowner.Label                 "MFG Owner")
        (mfgowner.HelpText              "Toggle display of manufacturing owner information")
        (mfgowner.AttachLeft            True)
        (mfgowner.AttachTop             True)
        (mfgowner.LeftOffset            8)
        (supres_btn.Label               "Suppressed Objects")
        (supres_btn.HelpText            "Toggle display of suppressed features and components")
        (supres_btn.AttachLeft          True)
        (supres_btn.AttachTop           True)
        (supres_btn.LeftOffset          8)
        (incomp_btn.Label               "Incomplete Objects")
        (incomp_btn.HelpText            "Toggle display of incomplete features")
        (incomp_btn.AttachLeft          True)
        (incomp_btn.AttachTop           True)
        (incomp_btn.LeftOffset          8)
        (exclud_btn.Label               "Excluded Objects")
        (exclud_btn.HelpText            "Toggle display of excluded components")
        (exclud_btn.AttachLeft          True)
        (exclud_btn.AttachTop           True)
        (exclud_btn.LeftOffset          8)
        (Label2.Label                   "Display")
        (Label2.AttachLeft              True)
        (Label2.AttachTop               True)
        (.Label                         "By Node Type")
        (.AttachLeft                    True)
        (.AttachTop                     True)
        (.TopOffset                     4)
        (.LeftOffset                    4)
        (.RightOffset                   4)
        (.Layout
            (Grid (Rows 0 0 0 1 1 1 1 1) (Cols 1)
                Label2
                feat_btn
                note_btn
                mfgowner
                supres_btn
                incomp_btn
                exclud_btn
                blank
            )
        )
    )
)

(Layout Layout1
    (Components
        (PushButton                     ok_btn)
        (PushButton                     apply)
        (PushButton                     cancel_btn)
    )

    (Resources
        (ok_btn.Label                   "OK")
        (ok_btn.TopOffset               8)
        (ok_btn.BottomOffset            6)
        (ok_btn.LeftOffset              0)
        (ok_btn.RightOffset             6)
        (apply.Label                    "Apply")
        (apply.TopOffset                8)
        (apply.BottomOffset             6)
        (apply.LeftOffset               0)
        (apply.RightOffset              6)
        (cancel_btn.Label               "Close")
        (cancel_btn.TopOffset           8)
        (cancel_btn.BottomOffset        6)
        (cancel_btn.LeftOffset          0)
        (cancel_btn.RightOffset         8)
        (.AttachRight                   True)
        (.TopOffset                     0)
        (.BottomOffset                  0)
        (.LeftOffset                    0)
        (.RightOffset                   0)
        (.Layout
            (Grid (Rows 1) (Cols 1 1 1)
                ok_btn
                apply
                cancel_btn
            )
        )
    )
)
