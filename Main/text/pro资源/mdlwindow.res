!
!  20-MAY-97 H-03-11 DLT  $$1  Created.
!

(Dialog mdlwindow
    (Components
        (ProeWindow                     ProeWin)
        (PushButton                     Close)
        (PushButton                     Select)
	(Separator                      Sep)
    )

    (Resources
        (ProeWin.AttachLeft             True)
        (ProeWin.AttachRight            True)
        (ProeWin.AttachTop              True)
        (ProeWin.AttachBottom           True)
        (ProeWin.ProeWinWidth           14.000000)
        (ProeWin.ProeWinHeight          14.000000)
        (ProeWin.ProeWinType            16)
        (Close.VertOffset               2)
        (Close.Label                    "Close")
        (Select.TopOffset               1)
        (Select.Label                   "Select")
        (.Label                         "Model Preview")
        (.DefaultButton                 "Close")
        (.Layout
            (Grid (Rows 1 0 0) (Cols 1)
                ProeWin
		Sep
                (Grid (Rows 1) (Cols 1 1)
                    Close
                    Select
                )
            )
        )
    )
)
