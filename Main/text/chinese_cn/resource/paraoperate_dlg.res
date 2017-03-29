!UI对话框示例程序资源文件:Example7_1.res
(Dialog 参数操作
     (Components  !对话框元件声明
         (SubLayout                      Layout1)
         (SubLayout                      Layout2)
         (PushButton                     Cancel)
         (Separator                      Separator1)
        
     )
    (Resources    !对话框资源(属性定义)

         !Cancel按钮属性定义
         (Cancel.Label                   "Exit")
		 (Cancel.Bitmap                  "cl_cancel")
         (Cancel.TopOffset               4)
         (Cancel.BottomOffset            4)
         (Cancel.LeftOffset              4)
         (Cancel.RightOffset             4)
         (Separator1.TopOffset           4)
         (.Label                         "参数操作") !对话框标题
		 (.StartLocation                  3)
         (.Layout  	!对话框元件的布局
             (Grid (Rows 1 1 1 1) (Cols 1) 
               Layout1  
               Layout2  
		       Separator1                	
               Cancel        
             )
         )
     )
 )



(Layout Layout1
    (Components
		 (List                      ParamsList)
    )
    (Resources

         (ParamsList.AttachLeft		True) 
		 (ParamsList.TopOffset		4)
         (ParamsList.BottomOffset	4)
         (ParamsList.LeftOffset		2)
         (ParamsList.RightOffset	2)
         (ParamsList.Columns        15)
	     (.Label					"参数列表")
         (.Decorated                True)
         (.TopOffset                4)
         (.BottomOffset             4)
         (.LeftOffset               4)
         (.RightOffset              4)
         (.Layout
            (Grid (Rows 1) (Cols 1)
			   ParamsList
            )
        )
    )
)

(Layout Layout2
    (Components
        (Label								Param_NameLabel)
        (InputPanel							Param_Name)
        (Label								Param_ValueLabel)
        (InputPanel							Param_Value)
        (Label								Param_ValueTypeLabel)
		(OptionMenu                         Param_ValueType)
        (PushButton                         AddParam)
        (PushButton                         DelParam)
        (PushButton                         Regen)

    )
    (Resources
        (Param_NameLabel.Label				"名称")
        (Param_NameLabel.Columns			2)
        (Param_NameLabel.AttachLeft         True)
        (Param_NameLabel.RightOffset        4)
        (Param_NameLabel.LeftOffset         2)
  		(Param_NameLabel.TopOffset		    4)
        (Param_NameLabel.BottomOffset	    4)
        
        (Param_Name.AttachLeft              True)
		(Param_Name.LeftOffset              0)
  		(Param_Name.TopOffset		        4)
        (Param_Name.Columns                 11)

        (Param_ValueLabel.Label				"数值")
        (Param_ValueLabel.Columns			2)
        (Param_ValueLabel.AttachLeft        True)
        (Param_ValueLabel.RightOffset       2)
        (Param_ValueLabel.LeftOffset        4)
  		(Param_ValueLabel.TopOffset		    4)
        (Param_ValueLabel.BottomOffset	    4)

        (Param_Value.AttachLeft             True)
        (Param_Value.LeftOffset             0)
  		(Param_Value.TopOffset		        4)
        (Param_Value.Columns                11)

        (Param_ValueTypeLabel.Label			"类型")
        (Param_ValueTypeLabel.Columns		2)
        (Param_ValueTypeLabel.AttachLeft    True)
        (Param_ValueTypeLabel.RightOffset   4)
        (Param_ValueTypeLabel.LeftOffset    2)
        (Param_ValueTypeLabel.TopOffset		4)

        (Param_ValueType.AttachLeft         True)  
        (Param_ValueType.LeftOffset	        2)
        (Param_ValueType.BottomOffset	    4)
        (Param_ValueType.TopOffset			4)
        (Param_ValueType.BottomOffset	    4)
        (Param_ValueType.Names				"1"
											"2"
											"3")
        (Param_ValueType.Labels				"PRO_PARAM_DOUBLE"
											"PRO_PARAM_STRING"
											"PRO_PARAM_INTEGER")

		(Param_ValueType.Columns			10)
        (AddParam.Label                      "添加(&A)") 
        (DelParam.Label                      "删除(&D)")
        (Regen.Label                         "再生(&R)")

		(.Decorated              			True)
        (.Label								"参数设置")
		(.TopOffset                4)
        (.BottomOffset             4)
        (.LeftOffset               4)
        (.RightOffset              4)
        (.Layout
            (Grid (Rows 1 1 1 1) (Cols 1)
               (Grid (Rows 1) (Cols 1 1)
                   Param_NameLabel
                   Param_Name
			   )
               (Grid (Rows 1) (Cols 1 1)
                   Param_ValueLabel
                   Param_Value
			   )
               (Grid (Rows 1) (Cols 1 1)
                   Param_ValueTypeLabel
                   Param_ValueType
			   )
               (Grid (Rows 1) (Cols 1 1 1)
                   AddParam
                   DelParam
				   Regen
			   )
            )
        )
    )
)

