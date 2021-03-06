IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'RuiLiMdlDb')
	DROP DATABASE [RuiLiMdlDb]
GO

CREATE DATABASE [RuiLiMdlDb]  ON (NAME = N'RuiLiMdlDb_Data', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL\data\RuiLiMdlDb_Data.MDF' , SIZE = 3, FILEGROWTH = 10%) LOG ON (NAME = N'RuiLiMdlDb_Log', FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL\data\RuiLiMdlDb_Log.LDF' , SIZE = 13, FILEGROWTH = 10%)
 COLLATE Chinese_PRC_CI_AS
GO

exec sp_dboption N'RuiLiMdlDb', N'autoclose', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'bulkcopy', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'trunc. log', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'torn page detection', N'true'
GO

exec sp_dboption N'RuiLiMdlDb', N'read only', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'dbo use', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'single', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'autoshrink', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'ANSI null default', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'recursive triggers', N'true'
GO

exec sp_dboption N'RuiLiMdlDb', N'ANSI nulls', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'concat null yields null', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'cursor close on commit', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'default to local cursor', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'quoted identifier', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'ANSI warnings', N'false'
GO

exec sp_dboption N'RuiLiMdlDb', N'auto create statistics', N'true'
GO

exec sp_dboption N'RuiLiMdlDb', N'auto update statistics', N'true'
GO

use [RuiLiMdlDb]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmInsVariantTable_AsmInsInfoTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmInsVariantTable] DROP CONSTRAINT FK_AsmInsVariantTable_AsmInsInfoTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmPerformanceInsTable_AsmInsInfoTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmPerformanceInsTable] DROP CONSTRAINT FK_AsmPerformanceInsTable_AsmInsInfoTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmCompVariantTable_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmCompVariantTable] DROP CONSTRAINT FK_AsmCompVariantTable_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmDrwTable_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmDrwTable] DROP CONSTRAINT FK_AsmDrwTable_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmInsInfoTable_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmInsInfoTable] DROP CONSTRAINT FK_AsmInsInfoTable_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmMakeupTable_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmMakeupTable] DROP CONSTRAINT FK_AsmMakeupTable_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmPerformanceTable_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmPerformanceTable] DROP CONSTRAINT FK_AsmPerformanceTable_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmPic_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmPic] DROP CONSTRAINT FK_AsmPic_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmRelationTable_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmRelationTable] DROP CONSTRAINT FK_AsmRelationTable_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmRelativeDimParaTable_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmRelativeDimParaTable] DROP CONSTRAINT FK_AsmRelativeDimParaTable_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmVariantTable_AsmManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmVariantTable] DROP CONSTRAINT FK_AsmVariantTable_AsmManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmPerformanceInsTable_AsmPerformanceTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmPerformanceInsTable] DROP CONSTRAINT FK_AsmPerformanceInsTable_AsmPerformanceTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmTempCompVariantTable_AsmTempMakeupTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmTempCompVariantTable] DROP CONSTRAINT FK_AsmTempCompVariantTable_AsmTempMakeupTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmTempDrwTable_AsmTempManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmTempDrwTable] DROP CONSTRAINT FK_AsmTempDrwTable_AsmTempManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmTempMakeupTable_AsmTempManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmTempMakeupTable] DROP CONSTRAINT FK_AsmTempMakeupTable_AsmTempManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmTempPerformanceTable_AsmTempManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmTempPerformanceTable] DROP CONSTRAINT FK_AsmTempPerformanceTable_AsmTempManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmTempPic_AsmTempManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmTempPic] DROP CONSTRAINT FK_AsmTempPic_AsmTempManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmTempRelationTable_AsmTempManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmTempRelationTable] DROP CONSTRAINT FK_AsmTempRelationTable_AsmTempManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmTempVariantTable_AsmTempManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmTempVariantTable] DROP CONSTRAINT FK_AsmTempVariantTable_AsmTempManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_AsmInsVariantTable_AsmVariantTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[AsmInsVariantTable] DROP CONSTRAINT FK_AsmInsVariantTable_AsmVariantTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partInsPerfValueTable_PartInsTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partInsPerfValueTable] DROP CONSTRAINT FK_partInsPerfValueTable_PartInsTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partInsValueTable_PartInsTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partInsValueTable] DROP CONSTRAINT FK_partInsValueTable_PartInsTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_PartDrawing_temp_PartInsTable_temp]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[PartDrawing_temp] DROP CONSTRAINT FK_PartDrawing_temp_PartInsTable_temp
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_PartInsPerfValueTable_temp_PartInsTable_temp]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[PartInsPerfValueTable_temp] DROP CONSTRAINT FK_PartInsPerfValueTable_temp_PartInsTable_temp
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partDrawing_PartManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partDrawing] DROP CONSTRAINT FK_partDrawing_PartManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_PartInsTable_PartManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[PartInsTable] DROP CONSTRAINT FK_PartInsTable_PartManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partPerfparaTable_PartManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partPerfparaTable] DROP CONSTRAINT FK_partPerfparaTable_PartManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partPic_PartManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partPic] DROP CONSTRAINT FK_partPic_PartManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partRelationSet_PartManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partRelationSet] DROP CONSTRAINT FK_partRelationSet_PartManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_PartRelativeDimParaTable_PartManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[PartRelativeDimParaTable] DROP CONSTRAINT FK_PartRelativeDimParaTable_PartManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partVariantTable_PartManageTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partVariantTable] DROP CONSTRAINT FK_partVariantTable_PartManageTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_Part_Temp_Drawing_Part_Temp_Mdl_Info]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[Part_Temp_Drawing] DROP CONSTRAINT FK_Part_Temp_Drawing_Part_Temp_Mdl_Info
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_Part_Temp_Para_Info_Part_Temp_Mdl_Info]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[Part_Temp_Para_Info] DROP CONSTRAINT FK_Part_Temp_Para_Info_Part_Temp_Mdl_Info
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_Part_Temp_Perf_Part_Temp_Mdl_Info]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[Part_Temp_Perf] DROP CONSTRAINT FK_Part_Temp_Perf_Part_Temp_Mdl_Info
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_Part_Temp_Pic_Part_Temp_Mdl_Info]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[Part_Temp_Pic] DROP CONSTRAINT FK_Part_Temp_Pic_Part_Temp_Mdl_Info
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_Part_Temp_Rel_Part_Temp_Mdl_Info]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[Part_Temp_Rel] DROP CONSTRAINT FK_Part_Temp_Rel_Part_Temp_Mdl_Info
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partInsPerfValueTable_partPerfparaTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partInsPerfValueTable] DROP CONSTRAINT FK_partInsPerfValueTable_partPerfparaTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[FK_partInsValueTable_partVariantTable]') and OBJECTPROPERTY(id, N'IsForeignKey') = 1)
ALTER TABLE [dbo].[partInsValueTable] DROP CONSTRAINT FK_partInsValueTable_partVariantTable
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[del_asm_mdl]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[del_asm_mdl]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[delete_asm_temp_mdl]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[delete_asm_temp_mdl]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[delete_mdl]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[delete_mdl]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[delete_rv]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[delete_rv]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[删除]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)
drop trigger [dbo].[删除]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmCompVariantTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmCompVariantTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmDrwTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmDrwTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmInsInfoTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmInsInfoTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmInsVariantTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmInsVariantTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmMakeupTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmMakeupTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmManageTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmManageTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmPerformanceInsTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmPerformanceInsTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmPerformanceTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmPerformanceTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmPic]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmPic]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmRelationTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmRelationTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmRelativeDimParaTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmRelativeDimParaTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmTempCompVariantTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmTempCompVariantTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmTempDrwTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmTempDrwTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmTempMakeupTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmTempMakeupTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmTempManageTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmTempManageTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmTempPerformanceTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmTempPerformanceTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmTempPic]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmTempPic]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmTempRelationTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmTempRelationTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmTempVariantTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmTempVariantTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[AsmVariantTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[AsmVariantTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[MapcodeInfoTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[MapcodeInfoTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartDrawingTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartDrawingTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartDrawing_temp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartDrawing_temp]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartInsPerfValueTable_temp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartInsPerfValueTable_temp]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartInsTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartInsTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartInsTable_temp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartInsTable_temp]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartInsValue_temp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartInsValue_temp]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartInstanceTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartInstanceTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartManageTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartManageTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PartRelativeDimParaTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PartRelativeDimParaTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Part_Temp_Drawing]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Part_Temp_Drawing]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Part_Temp_Mdl_Info]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Part_Temp_Mdl_Info]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Part_Temp_Para_Info]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Part_Temp_Para_Info]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Part_Temp_Perf]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Part_Temp_Perf]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Part_Temp_Pic]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Part_Temp_Pic]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[Part_Temp_Rel]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[Part_Temp_Rel]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[PerformeInsTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[PerformeInsTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ProductDrawingTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ProductDrawingTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ProductDrawingTable_temp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ProductDrawingTable_temp]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ProductInsTable_temp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ProductInsTable_temp]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ProductMakeupTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ProductMakeupTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ProductManageTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ProductManageTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ProductPerValueTable_temp]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ProductPerValueTable_temp]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[ProductTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[ProductTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[partDrawing]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[partDrawing]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[partInsPerfValueTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[partInsPerfValueTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[partInsValueTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[partInsValueTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[partPerfparaTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[partPerfparaTable]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[partPic]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[partPic]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[partRelationSet]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[partRelationSet]
GO

if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[partVariantTable]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
drop table [dbo].[partVariantTable]
GO

if not exists (select * from master.dbo.syslogins where loginname = N'aspx')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'aspx', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'cbs')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'RuiLiMdlDb', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'cbs', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'fat')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'fat', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'fat', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'hqy')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'user_db', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'hqy', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'hxyy')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'hxyy', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'ibucmcourse')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'ibucmcourse', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'myhomeAdmin')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'myhomeAdmin', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'net')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'net', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'NewIbucmAdmin')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'NewIbucmAdmin', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'ng')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'ng', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'PDM')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'PDM', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'scm')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'asp', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'scm', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'SunWindCityAdmin')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'SunWindCityAdmin', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'tree')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'tree', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'tree', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'web')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'web', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'web', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'xd')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'xd', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'xiao')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'master', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'xiao', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'xzqh')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'yuanshi', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'xzqh', null, @logindb, @loginlang
END
GO

if not exists (select * from master.dbo.syslogins where loginname = N'zjucareer')
BEGIN
	declare @logindb nvarchar(132), @loginlang nvarchar(132) select @logindb = N'theCs', @loginlang = N'简体中文'
	if @logindb is null or not exists (select * from master.dbo.sysdatabases where name = @logindb)
		select @logindb = N'master'
	if @loginlang is null or (not exists (select * from master.dbo.syslanguages where name = @loginlang) and @loginlang <> N'us_english')
		select @loginlang = @@language
	exec sp_addlogin N'zjucareer', null, @logindb, @loginlang
END
GO

exec sp_addsrvrolemember N'cbs', sysadmin
GO

exec sp_addsrvrolemember N'fat', sysadmin
GO

exec sp_addsrvrolemember N'PDM', sysadmin
GO

exec sp_addsrvrolemember N'scm', sysadmin
GO

exec sp_addsrvrolemember N'web', sysadmin
GO

exec sp_addsrvrolemember N'xd', sysadmin
GO

exec sp_addsrvrolemember N'xiao', sysadmin
GO

exec sp_addsrvrolemember N'zjucareer', sysadmin
GO

exec sp_addsrvrolemember N'cbs', securityadmin
GO

exec sp_addsrvrolemember N'PDM', securityadmin
GO

exec sp_addsrvrolemember N'xd', securityadmin
GO

exec sp_addsrvrolemember N'xiao', securityadmin
GO

exec sp_addsrvrolemember N'zjucareer', securityadmin
GO

exec sp_addsrvrolemember N'cbs', serveradmin
GO

exec sp_addsrvrolemember N'PDM', serveradmin
GO

exec sp_addsrvrolemember N'web', serveradmin
GO

exec sp_addsrvrolemember N'xd', serveradmin
GO

exec sp_addsrvrolemember N'xiao', serveradmin
GO

exec sp_addsrvrolemember N'zjucareer', serveradmin
GO

exec sp_addsrvrolemember N'cbs', setupadmin
GO

exec sp_addsrvrolemember N'PDM', setupadmin
GO

exec sp_addsrvrolemember N'xd', setupadmin
GO

exec sp_addsrvrolemember N'xiao', setupadmin
GO

exec sp_addsrvrolemember N'zjucareer', setupadmin
GO

exec sp_addsrvrolemember N'cbs', processadmin
GO

exec sp_addsrvrolemember N'PDM', processadmin
GO

exec sp_addsrvrolemember N'xd', processadmin
GO

exec sp_addsrvrolemember N'xiao', processadmin
GO

exec sp_addsrvrolemember N'zjucareer', processadmin
GO

exec sp_addsrvrolemember N'cbs', diskadmin
GO

exec sp_addsrvrolemember N'PDM', diskadmin
GO

exec sp_addsrvrolemember N'xd', diskadmin
GO

exec sp_addsrvrolemember N'xiao', diskadmin
GO

exec sp_addsrvrolemember N'zjucareer', diskadmin
GO

exec sp_addsrvrolemember N'cbs', dbcreator
GO

exec sp_addsrvrolemember N'PDM', dbcreator
GO

exec sp_addsrvrolemember N'xd', dbcreator
GO

exec sp_addsrvrolemember N'xiao', dbcreator
GO

exec sp_addsrvrolemember N'zjucareer', dbcreator
GO

exec sp_addsrvrolemember N'cbs', bulkadmin
GO

exec sp_addsrvrolemember N'PDM', bulkadmin
GO

exec sp_addsrvrolemember N'xd', bulkadmin
GO

exec sp_addsrvrolemember N'xiao', bulkadmin
GO

exec sp_addsrvrolemember N'zjucareer', bulkadmin
GO

if not exists (select * from dbo.sysusers where name = N'cbs' and uid < 16382)
	EXEC sp_grantdbaccess N'cbs', N'cbs'
GO

GRANT  CREATE FUNCTION ,  CREATE TABLE ,  CREATE VIEW ,  CREATE PROCEDURE ,  DUMP DATABASE ,  CREATE DEFAULT ,  DUMP TRANSACTION ,  CREATE RULE  TO [cbs]
GO

exec sp_addrolemember N'db_accessadmin', N'cbs'
GO

exec sp_addrolemember N'db_backupoperator', N'cbs'
GO

exec sp_addrolemember N'db_datareader', N'cbs'
GO

exec sp_addrolemember N'db_datawriter', N'cbs'
GO

exec sp_addrolemember N'db_ddladmin', N'cbs'
GO

exec sp_addrolemember N'db_denydatareader', N'cbs'
GO

exec sp_addrolemember N'db_denydatawriter', N'cbs'
GO

exec sp_addrolemember N'db_owner', N'cbs'
GO

exec sp_addrolemember N'db_securityadmin', N'cbs'
GO

CREATE TABLE [dbo].[AsmCompVariantTable] (
	[AsmNum] [int] NOT NULL ,
	[CompNum] [int] NOT NULL ,
	[CompMdlNum] [int] NOT NULL ,
	[CompInsNum] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmDrwTable] (
	[AsmNum] [int] NOT NULL ,
	[DrwNum] [int] NOT NULL ,
	[DrwFileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DrwNote] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmInsInfoTable] (
	[AsmNum] [int] NOT NULL ,
	[InsNum] [int] NOT NULL ,
	[InsName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[InsCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[SavePlace] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[InsCreateTime] [datetime] NULL ,
	[InsCreaterName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[InsCreateDpt] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[InsNote] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmInsVariantTable] (
	[AsmNum] [int] NOT NULL ,
	[InsNum] [int] NOT NULL ,
	[Num] [int] NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmMakeupTable] (
	[AsmNum] [int] NOT NULL ,
	[PartNum] [int] NOT NULL ,
	[PartMdlNum] [int] NOT NULL ,
	[DefInsNum] [int] NOT NULL ,
	[Type] [char] (3) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[UpMdlNum] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[Code] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmManageTable] (
	[Number] [int] IDENTITY (1, 1) NOT NULL ,
	[FatherNumber] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Type] [char] (3) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[MdlCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Datatime] [datetime] NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[FileDirectory] [varchar] (200) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[MdlerName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[MdlerDepartment] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmPerformanceInsTable] (
	[AsmNum] [int] NOT NULL ,
	[InsNum] [int] NOT NULL ,
	[Num] [int] NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmPerformanceTable] (
	[AsmNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmPic] (
	[AsmNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[IsMain] [int] NOT NULL ,
	[File] [image] NULL 
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmRelationTable] (
	[AsmNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Relation] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmRelativeDimParaTable] (
	[ProductMdlNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[PostNum] [int] NOT NULL ,
	[Type] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[IdNum] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimSymbol] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[SubType] [char] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmTempCompVariantTable] (
	[AsmNum] [int] NOT NULL ,
	[CompNum] [int] NOT NULL ,
	[CompMdlNum] [int] NOT NULL ,
	[CompInsNum] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmTempDrwTable] (
	[AsmNum] [int] NOT NULL ,
	[DrwNum] [int] NOT NULL ,
	[DrwFileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DrwNote] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmTempMakeupTable] (
	[AsmNum] [int] NOT NULL ,
	[PartNum] [int] NOT NULL ,
	[PartMdlNum] [int] NOT NULL ,
	[DefInsNum] [int] NOT NULL ,
	[Type] [char] (3) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[UpMdlNum] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[Code] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmTempManageTable] (
	[Number] [int] IDENTITY (1, 1) NOT NULL ,
	[FatherNumber] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Type] [char] (3) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[MdlCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Datatime] [datetime] NOT NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[FileDirectory] [varchar] (200) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[MdlerName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[MdlerDepartment] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[MdlFatherNum] [int] NULL ,
	[MdlType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmTempPerformanceTable] (
	[AsmNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmTempPic] (
	[AsmNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[IsMain] [int] NOT NULL ,
	[File] [image] NULL 
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmTempRelationTable] (
	[AsmNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Relation] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmTempVariantTable] (
	[AsmNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[PostVariant] [int] NOT NULL ,
	[Type] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[IdNum] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimSymbol] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[ParaFeatName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[SubType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[UpLimit] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[DwLimit] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AsmVariantTable] (
	[AsmNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[PostVariant] [int] NOT NULL ,
	[Type] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[IdNum] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimSymbol] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[ParaFeatName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[SubType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[UpLimit] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[DwLimit] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[MapcodeInfoTable] (
	[MapInfo] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ModelNum] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ModelType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[SavePlace] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Designer] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Remark] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartDrawingTable] (
	[MapCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DrawingName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartDrawing_temp] (
	[PartMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ProductMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DrawingName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartInsPerfValueTable_temp] (
	[PartMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ProductMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Num] [int] NOT NULL ,
	[PartNum] [int] NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartInsTable] (
	[PrtNum] [int] NOT NULL ,
	[InsNum] [int] NOT NULL ,
	[InsName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[InsCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[SavePlace] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[InsNote] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[InsCreateTime] [datetime] NULL ,
	[InsCreaterName] [varchar] (30) COLLATE Chinese_PRC_CI_AS NULL ,
	[InsCreateDpt] [varchar] (30) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartInsTable_temp] (
	[MapCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ModelNum] [int] NOT NULL ,
	[InsName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[SavePlace] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ProductMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Designer] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartInsValue_temp] (
	[PartCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Num] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartInstanceTable] (
	[PartMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimenNum] [int] NOT NULL ,
	[DimenName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimenValue] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimenType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartManageTable] (
	[Number] [int] IDENTITY (1, 1) NOT NULL ,
	[FatherNumber] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Type] [char] (3) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Datatime] [datetime] NOT NULL ,
	[MdlCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[FileDirectory] [varchar] (200) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[MdlerName] [varchar] (20) COLLATE Chinese_PRC_CI_AS NULL ,
	[MdlerDepartment] [varchar] (20) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartRelativeDimParaTable] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Type] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[IdNum] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimSymbol] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[SubType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Part_Temp_Drawing] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Part_Temp_Mdl_Info] (
	[Number] [int] IDENTITY (1, 1) NOT NULL ,
	[FatherNum] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Type] [char] (3) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Datatime] [datetime] NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[FileDirectory] [varchar] (200) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[MdlerName] [varchar] (20) COLLATE Chinese_PRC_CI_AS NULL ,
	[MdlerId] [int] NULL ,
	[MdlerDepartment] [varchar] (20) COLLATE Chinese_PRC_CI_AS NULL ,
	[MdlFatherNum] [int] NULL ,
	[MdlCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[MdlType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Status] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Part_Temp_Para_Info] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Type] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[IdNum] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimSymbol] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[ParaFeatName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[SubType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[UpLimit] [varchar] (20) COLLATE Chinese_PRC_CI_AS NULL ,
	[DwLimit] [varchar] (20) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Part_Temp_Perf] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Part_Temp_Pic] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[IsMain] [int] NOT NULL ,
	[File] [image] NULL 
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

CREATE TABLE [dbo].[Part_Temp_Rel] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Relation] [varchar] (100) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PerformeInsTable] (
	[MapCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[PerfValue] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ProductDrawingTable] (
	[MapCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DrawingName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ProductDrawingTable_temp] (
	[MapCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DrawingName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ProductInsTable_temp] (
	[MapCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ModelNum] [int] NOT NULL ,
	[InsName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[SavePlace] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ProductMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Manager] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ProductMakeupTable] (
	[ProductMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Num] [int] NOT NULL ,
	[UpNum] [int] NOT NULL ,
	[MdlNum] [int] NOT NULL ,
	[PartMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ProductManageTable] (
	[ModelNum] [int] NOT NULL ,
	[FatherNum] [int] NOT NULL ,
	[ProductName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Type] [char] (10) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ModelCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[CreateDate] [datetime] NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DirectoryName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Creator] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[CreateDepartment] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ProductPerValueTable_temp] (
	[MapCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ProductMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Num] [int] NOT NULL ,
	[ModelNum] [int] NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ProductTable] (
	[ProductMap] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ModelNumFrom] [int] NOT NULL ,
	[ProductName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[GroupFrom] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[StandFrom] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[ModelCode] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[partDrawing] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[FileName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[partInsPerfValueTable] (
	[PrtNum] [int] NOT NULL ,
	[InsNum] [int] NOT NULL ,
	[Num] [int] NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[partInsValueTable] (
	[PrtNum] [int] NOT NULL ,
	[InsNum] [int] NOT NULL ,
	[Num] [int] NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[partPerfparaTable] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[partPic] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL ,
	[IsMain] [int] NOT NULL ,
	[File] [image] NULL 
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO

CREATE TABLE [dbo].[partRelationSet] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Relation] [varchar] (100) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[partVariantTable] (
	[PrtNum] [int] NOT NULL ,
	[Number] [int] NOT NULL ,
	[Name] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Type] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[IdNum] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[DimSymbol] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[ParaFeatName] [varchar] (50) COLLATE Chinese_PRC_CI_AS NULL ,
	[SubType] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[Value] [varchar] (50) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[UpLimit] [varchar] (20) COLLATE Chinese_PRC_CI_AS NULL ,
	[DwLimit] [varchar] (20) COLLATE Chinese_PRC_CI_AS NULL ,
	[Note] [varchar] (150) COLLATE Chinese_PRC_CI_AS NULL 
) ON [PRIMARY]
GO

ALTER TABLE [dbo].[AsmCompVariantTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempCompVariantTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[CompNum],
		[CompMdlNum],
		[CompInsNum]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmDrwTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempDrwTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[DrwNum]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmInsInfoTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmInsInfoTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[InsNum]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmInsVariantTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmInsVariantTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[InsNum],
		[Num]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmMakeupTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempMakeupTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[PartNum]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmManageTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempManageTable] PRIMARY KEY  CLUSTERED 
	(
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmPerformanceInsTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmPerformanceInsTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[InsNum],
		[Num]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmPerformanceTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempPerformanceTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmPic] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmPic] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmRelationTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempRelationTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmRelativeDimParaTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmRelativeDimParaTable] PRIMARY KEY  CLUSTERED 
	(
		[ProductMdlNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmTempCompVariantTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempCompVariantTable_1] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[CompNum],
		[CompMdlNum],
		[CompInsNum]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmTempDrwTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempDrwTable_1] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[DrwNum]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmTempMakeupTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempMakeupTable_1] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[PartNum]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmTempManageTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempManageTable_1] PRIMARY KEY  CLUSTERED 
	(
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmTempPerformanceTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempPerformanceTable_1] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmTempPic] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempPic] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmTempRelationTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempRelationTable_1] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmTempVariantTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempVariantTable_1] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmVariantTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_AsmTempVariantTable] PRIMARY KEY  CLUSTERED 
	(
		[AsmNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[MapcodeInfoTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_MapcodeInfo] PRIMARY KEY  CLUSTERED 
	(
		[MapInfo]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartDrawingTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartDrawingTable] PRIMARY KEY  CLUSTERED 
	(
		[MapCode],
		[DrawingName]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartDrawing_temp] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartDrawing_temp] PRIMARY KEY  CLUSTERED 
	(
		[PartMap],
		[ProductMap],
		[DrawingName]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartInsPerfValueTable_temp] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartInsPerfValueTable_temp] PRIMARY KEY  CLUSTERED 
	(
		[PartMap],
		[ProductMap],
		[Num]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartInsTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartInsTable] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[InsNum]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartInsTable_temp] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartInsTable_temp] PRIMARY KEY  CLUSTERED 
	(
		[MapCode],
		[ProductMap]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartInsValue_temp] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartInsValue_temp] PRIMARY KEY  CLUSTERED 
	(
		[PartCode],
		[Num]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartManageTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartManageTable] PRIMARY KEY  CLUSTERED 
	(
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartRelativeDimParaTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartRelativeDimParaTable] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Part_Temp_Drawing] WITH NOCHECK ADD 
	CONSTRAINT [PK_Part_Temp_Drawing] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Part_Temp_Mdl_Info] WITH NOCHECK ADD 
	CONSTRAINT [PK_Part_Temp_Mdl_Info] PRIMARY KEY  CLUSTERED 
	(
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Part_Temp_Para_Info] WITH NOCHECK ADD 
	CONSTRAINT [PK_Part_Temp_Para_Info] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Part_Temp_Perf] WITH NOCHECK ADD 
	CONSTRAINT [PK_Part_Temp_Perf] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Part_Temp_Pic] WITH NOCHECK ADD 
	CONSTRAINT [PK_Part_Temp_Pic] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Part_Temp_Rel] WITH NOCHECK ADD 
	CONSTRAINT [PK_Part_Temp_Rel] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PerformeInsTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_PerformeInsTable] PRIMARY KEY  CLUSTERED 
	(
		[MapCode],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ProductDrawingTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_ProductDrawingTable] PRIMARY KEY  CLUSTERED 
	(
		[MapCode],
		[DrawingName]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ProductDrawingTable_temp] WITH NOCHECK ADD 
	CONSTRAINT [PK_ProductDrawingTable_temp] PRIMARY KEY  CLUSTERED 
	(
		[MapCode],
		[DrawingName]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ProductInsTable_temp] WITH NOCHECK ADD 
	CONSTRAINT [PK_ProductInsTable_temp] PRIMARY KEY  CLUSTERED 
	(
		[MapCode],
		[ProductMap]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ProductMakeupTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_ProductMakeupTable] PRIMARY KEY  CLUSTERED 
	(
		[ProductMap],
		[Num]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ProductPerValueTable_temp] WITH NOCHECK ADD 
	CONSTRAINT [PK_ProductPerValueTable_temp] PRIMARY KEY  CLUSTERED 
	(
		[MapCode],
		[ProductMap],
		[Num]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ProductTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_ProductTable] PRIMARY KEY  CLUSTERED 
	(
		[ProductMap]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[partDrawing] WITH NOCHECK ADD 
	CONSTRAINT [PK_partDrawing] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number],
		[FileName]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[partInsPerfValueTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_partInsPerfValueTable] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[InsNum],
		[Num]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[partInsValueTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_partInsValueTable] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[InsNum],
		[Num]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[partPerfparaTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_partPerfparaTable] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[partPic] WITH NOCHECK ADD 
	CONSTRAINT [PK_partPic] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[partRelationSet] WITH NOCHECK ADD 
	CONSTRAINT [PK_partRelationSet] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[partVariantTable] WITH NOCHECK ADD 
	CONSTRAINT [PK_partVariantTable] PRIMARY KEY  CLUSTERED 
	(
		[PrtNum],
		[Number]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AsmMakeupTable] WITH NOCHECK ADD 
	CONSTRAINT [DF_AsmTempMakeupTable_PartMdlNum] DEFAULT ((-1)) FOR [PartMdlNum],
	CONSTRAINT [DF_AsmTempMakeupTable_DefInsNum] DEFAULT ((-1)) FOR [DefInsNum],
	CONSTRAINT [DF_AsmTempMakeupTable_Type] DEFAULT ('prt') FOR [Type],
	CONSTRAINT [CK_AsmTempMakeupTable] CHECK ([Type] = 'prt' or [Type] = 'asm')
GO

ALTER TABLE [dbo].[AsmManageTable] WITH NOCHECK ADD 
	CONSTRAINT [DF_AsmTempManageTable_Type] DEFAULT ('typ') FOR [Type],
	CONSTRAINT [CK_AsmTempManageTable] CHECK ([Type] = 'typ' or [Type] = 'mdl')
GO

ALTER TABLE [dbo].[AsmRelativeDimParaTable] WITH NOCHECK ADD 
	CONSTRAINT [CK_AsmRelativeDimParaTable] CHECK ([Type] = 'dim' or [Type] = 'para')
GO

ALTER TABLE [dbo].[PartManageTable] WITH NOCHECK ADD 
	CONSTRAINT [DF_PartManageTable_FatherNumber] DEFAULT (0) FOR [FatherNumber],
	CONSTRAINT [DF_PartManageTable_Type] DEFAULT ('typ') FOR [Type],
	CONSTRAINT [CK_PartManageTable_1] CHECK ([Type] = 'mdl' or [Type] = 'typ')
GO

ALTER TABLE [dbo].[Part_Temp_Mdl_Info] WITH NOCHECK ADD 
	CONSTRAINT [DF_Part_Temp_Mdl_Info_Type] DEFAULT ('mdl') FOR [Type],
	CONSTRAINT [DF_Part_Temp_Mdl_Info_Staus] DEFAULT (0) FOR [Status],
	CONSTRAINT [CK_Part_Temp_Mdl_Info] CHECK ([Type] = 'mdl' or [Type] = 'wrk')
GO

ALTER TABLE [dbo].[partPic] WITH NOCHECK ADD 
	CONSTRAINT [DF_partPic_IsMain] DEFAULT (0) FOR [IsMain],
	CONSTRAINT [CK_partPic] CHECK ([IsMain] = 0 or [IsMain] = 1)
GO

 CREATE  INDEX [IX_PartManageTable] ON [dbo].[PartManageTable]([Number]) ON [PRIMARY]
GO

ALTER TABLE [dbo].[AsmCompVariantTable] ADD 
	CONSTRAINT [FK_AsmCompVariantTable_AsmManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmDrwTable] ADD 
	CONSTRAINT [FK_AsmDrwTable_AsmManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmInsInfoTable] ADD 
	CONSTRAINT [FK_AsmInsInfoTable_AsmManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmInsVariantTable] ADD 
	CONSTRAINT [FK_AsmInsVariantTable_AsmInsInfoTable] FOREIGN KEY 
	(
		[AsmNum],
		[InsNum]
	) REFERENCES [dbo].[AsmInsInfoTable] (
		[AsmNum],
		[InsNum]
	) ON DELETE CASCADE ,
	CONSTRAINT [FK_AsmInsVariantTable_AsmVariantTable] FOREIGN KEY 
	(
		[AsmNum],
		[Num]
	) REFERENCES [dbo].[AsmVariantTable] (
		[AsmNum],
		[Number]
	)
GO

ALTER TABLE [dbo].[AsmMakeupTable] ADD 
	CONSTRAINT [FK_AsmMakeupTable_AsmManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmPerformanceInsTable] ADD 
	CONSTRAINT [FK_AsmPerformanceInsTable_AsmInsInfoTable] FOREIGN KEY 
	(
		[AsmNum],
		[InsNum]
	) REFERENCES [dbo].[AsmInsInfoTable] (
		[AsmNum],
		[InsNum]
	) ON DELETE CASCADE ,
	CONSTRAINT [FK_AsmPerformanceInsTable_AsmPerformanceTable] FOREIGN KEY 
	(
		[AsmNum],
		[Num]
	) REFERENCES [dbo].[AsmPerformanceTable] (
		[AsmNum],
		[Number]
	)
GO

ALTER TABLE [dbo].[AsmPerformanceTable] ADD 
	CONSTRAINT [FK_AsmPerformanceTable_AsmManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmPic] ADD 
	CONSTRAINT [FK_AsmPic_AsmManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmRelationTable] ADD 
	CONSTRAINT [FK_AsmRelationTable_AsmManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmRelativeDimParaTable] ADD 
	CONSTRAINT [FK_AsmRelativeDimParaTable_AsmManageTable] FOREIGN KEY 
	(
		[ProductMdlNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmTempCompVariantTable] ADD 
	CONSTRAINT [FK_AsmTempCompVariantTable_AsmTempMakeupTable] FOREIGN KEY 
	(
		[AsmNum],
		[CompNum]
	) REFERENCES [dbo].[AsmTempMakeupTable] (
		[AsmNum],
		[PartNum]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmTempDrwTable] ADD 
	CONSTRAINT [FK_AsmTempDrwTable_AsmTempManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmTempManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmTempMakeupTable] ADD 
	CONSTRAINT [FK_AsmTempMakeupTable_AsmTempManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmTempManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmTempPerformanceTable] ADD 
	CONSTRAINT [FK_AsmTempPerformanceTable_AsmTempManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmTempManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmTempPic] ADD 
	CONSTRAINT [FK_AsmTempPic_AsmTempManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmTempManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmTempRelationTable] ADD 
	CONSTRAINT [FK_AsmTempRelationTable_AsmTempManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmTempManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmTempVariantTable] ADD 
	CONSTRAINT [FK_AsmTempVariantTable_AsmTempManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmTempManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[AsmVariantTable] ADD 
	CONSTRAINT [FK_AsmVariantTable_AsmManageTable] FOREIGN KEY 
	(
		[AsmNum]
	) REFERENCES [dbo].[AsmManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[PartDrawing_temp] ADD 
	CONSTRAINT [FK_PartDrawing_temp_PartInsTable_temp] FOREIGN KEY 
	(
		[PartMap],
		[ProductMap]
	) REFERENCES [dbo].[PartInsTable_temp] (
		[MapCode],
		[ProductMap]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[PartInsPerfValueTable_temp] ADD 
	CONSTRAINT [FK_PartInsPerfValueTable_temp_PartInsTable_temp] FOREIGN KEY 
	(
		[PartMap],
		[ProductMap]
	) REFERENCES [dbo].[PartInsTable_temp] (
		[MapCode],
		[ProductMap]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[PartInsTable] ADD 
	CONSTRAINT [FK_PartInsTable_PartManageTable] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[PartManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[PartRelativeDimParaTable] ADD 
	CONSTRAINT [FK_PartRelativeDimParaTable_PartManageTable] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[PartManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[Part_Temp_Drawing] ADD 
	CONSTRAINT [FK_Part_Temp_Drawing_Part_Temp_Mdl_Info] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[Part_Temp_Mdl_Info] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[Part_Temp_Para_Info] ADD 
	CONSTRAINT [FK_Part_Temp_Para_Info_Part_Temp_Mdl_Info] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[Part_Temp_Mdl_Info] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[Part_Temp_Perf] ADD 
	CONSTRAINT [FK_Part_Temp_Perf_Part_Temp_Mdl_Info] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[Part_Temp_Mdl_Info] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[Part_Temp_Pic] ADD 
	CONSTRAINT [FK_Part_Temp_Pic_Part_Temp_Mdl_Info] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[Part_Temp_Mdl_Info] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[Part_Temp_Rel] ADD 
	CONSTRAINT [FK_Part_Temp_Rel_Part_Temp_Mdl_Info] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[Part_Temp_Mdl_Info] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[partDrawing] ADD 
	CONSTRAINT [FK_partDrawing_PartManageTable] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[PartManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[partInsPerfValueTable] ADD 
	CONSTRAINT [FK_partInsPerfValueTable_PartInsTable] FOREIGN KEY 
	(
		[PrtNum],
		[InsNum]
	) REFERENCES [dbo].[PartInsTable] (
		[PrtNum],
		[InsNum]
	) ON DELETE CASCADE ,
	CONSTRAINT [FK_partInsPerfValueTable_partPerfparaTable] FOREIGN KEY 
	(
		[PrtNum],
		[Num]
	) REFERENCES [dbo].[partPerfparaTable] (
		[PrtNum],
		[Number]
	)
GO

ALTER TABLE [dbo].[partInsValueTable] ADD 
	CONSTRAINT [FK_partInsValueTable_PartInsTable] FOREIGN KEY 
	(
		[PrtNum],
		[InsNum]
	) REFERENCES [dbo].[PartInsTable] (
		[PrtNum],
		[InsNum]
	) ON DELETE CASCADE ,
	CONSTRAINT [FK_partInsValueTable_partVariantTable] FOREIGN KEY 
	(
		[PrtNum],
		[Num]
	) REFERENCES [dbo].[partVariantTable] (
		[PrtNum],
		[Number]
	)
GO

ALTER TABLE [dbo].[partPerfparaTable] ADD 
	CONSTRAINT [FK_partPerfparaTable_PartManageTable] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[PartManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[partPic] ADD 
	CONSTRAINT [FK_partPic_PartManageTable] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[PartManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[partRelationSet] ADD 
	CONSTRAINT [FK_partRelationSet_PartManageTable] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[PartManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

ALTER TABLE [dbo].[partVariantTable] ADD 
	CONSTRAINT [FK_partVariantTable_PartManageTable] FOREIGN KEY 
	(
		[PrtNum]
	) REFERENCES [dbo].[PartManageTable] (
		[Number]
	) ON DELETE CASCADE 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE TRIGGER del_asm_mdl ON [dbo].[AsmManageTable] 
FOR INSERT, UPDATE, DELETE 
AS
IF (SELECT COUNT(*) FROM DELETED) > 0
DELETE FROM [AsmManageTable]  WHERE [AsmManageTable].[FatherNumber]   IN (select number from deleted)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE TRIGGER delete_asm_temp_mdl  ON dbo.AsmTempManageTable 
FOR DELETE 

AS
IF (SELECT COUNT(*) FROM DELETED) > 0
DELETE FROM [AsmTempManageTable] WHERE [AsmTempManageTable].[FatherNumber]  IN (select number from deleted)






GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE TRIGGER delete_mdl  ON [dbo].[PartManageTable] 
FOR DELETE 
AS
if (select count(*) from deleted)>0
DELETE FROM [PartManageTable] WHERE [PartManageTable].[FatherNumber] IN (select number from deleted)









GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER ON 
GO
SET ANSI_NULLS ON 
GO

CREATE TRIGGER delete_rv ON dbo.Part_Temp_Mdl_Info 
FOR DELETE 
AS
if (select count(*) from deleted) > 0
DELETE FROM [Part_Temp_Mdl_Info] WHERE [Part_Temp_Mdl_Info].[FatherNum] IN (select number from deleted)


GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

CREATE TRIGGER [删除] ON [dbo].[ProductInsTable_temp] 
FOR DELETE 
AS 
    DELETE FROM  [ProductDrawingTable_temp] 
            where [ProductDrawingTable_temp].MapCode In (SELECT ProductMap FROM deleted)
    DELETE FROM  [ProductPerValueTable_temp] 
            where [ProductPerValueTable_temp].ProductMap In (SELECT ProductMap FROM deleted)
     DELETE FROM  [PartInsTable_temp] 
            where [PartInsTable_temp].ProductMap In (SELECT ProductMap FROM deleted)
GO
SET QUOTED_IDENTIFIER OFF 
GO
SET ANSI_NULLS ON 
GO

