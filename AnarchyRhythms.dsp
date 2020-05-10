# Microsoft Developer Studio Project File - Name="AnarchyRhythms" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AnarchyRhythms - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AnarchyRhythms.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AnarchyRhythms.mak" CFG="AnarchyRhythms - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AnarchyRhythms - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AnarchyRhythms - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AnarchyRhythms - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I "../source/common" /I "../source/ADelay" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D WINDOWS=1 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "resource" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib LibCMT.LIB vstgui.lib htmlhelp.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"library"
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=deploy plugin to wavelab folder
PostBuild_Cmds=copy Release\AnarchyRhythms.dll C:\Progra~1\Steinberg\Wavelab\System\plugins\AnarchyRhythms_build.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "AnarchyRhythms - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /Zi /Od /I "../source/common" /I "../source/ADelay" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D WINDOWS=1 /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib LibCMTD.LIB vstgui.lib htmlhelp.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"library"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "AnarchyRhythms - Win32 Release"
# Name "AnarchyRhythms - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RESOURCE\AnarchyRhythms.RC2
# End Source File
# Begin Source File

SOURCE=.\resource\DelayEditGUI.rc

!IF  "$(CFG)" == "AnarchyRhythms - Win32 Release"

# ADD BASE RSC /l 0x809 /i "resource"
# SUBTRACT BASE RSC /i "resource"
# ADD RSC /l 0x809 /i "resource"
# SUBTRACT RSC /i "resource"

!ELSEIF  "$(CFG)" == "AnarchyRhythms - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Resource\faded00128.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\faded00129.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\IOSlider.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\KnobLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\KnobSmall.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\LevelSlider.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource\logoLighter.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\MixSlider.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\PatternDown.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\PatternDownDisabled.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\PatternUp.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\PatternUpDisabled.bmp
# End Source File
# Begin Source File

SOURCE=".\RESOURCE\Rhythms Logo.bmp"
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\ShuffleSlider.bmp
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\TEMPLATE00128.BMP
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\TEMPLATE00129.BMP
# End Source File
# End Group
# Begin Group "VST Framework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ADelayEditMain.cpp
# End Source File
# Begin Source File

SOURCE=.\AEffect.h
# End Source File
# Begin Source File

SOURCE=.\aeffectx.h
# End Source File
# Begin Source File

SOURCE=.\AEffEditor.hpp
# End Source File
# Begin Source File

SOURCE=.\AnarchyRhythms.def
# End Source File
# Begin Source File

SOURCE=.\AudioEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioEffect.hpp
# End Source File
# Begin Source File

SOURCE=.\audioeffectx.cpp
# End Source File
# Begin Source File

SOURCE=.\audioeffectx.h
# End Source File
# Begin Source File

SOURCE=.\BaseDLLEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseDLLEdit.hpp
# End Source File
# Begin Source File

SOURCE=.\vstcontrols.h
# End Source File
# Begin Source File

SOURCE=.\vstgui.h
# End Source File
# End Group
# Begin Group "AnarchyRhythms Core"

# PROP Default_Filter ""
# Begin Group "backward compatibility"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\externalstatedata_v1.cpp
# End Source File
# Begin Source File

SOURCE=.\EXTERNALSTATEDATA_v1.H
# End Source File
# Begin Source File

SOURCE=.\midimap_v1.cpp
# End Source File
# Begin Source File

SOURCE=.\midimap_v1.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BandPass.cpp
# End Source File
# Begin Source File

SOURCE=.\BandPass.h
# End Source File
# Begin Source File

SOURCE=.\BaseDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseDLL.hpp
# End Source File
# Begin Source File

SOURCE=.\Compression.cpp
# End Source File
# Begin Source File

SOURCE=.\Compression.h
# End Source File
# Begin Source File

SOURCE=.\EXTERNALSTATEDATA_v2.CPP
# End Source File
# Begin Source File

SOURCE=.\EXTERNALSTATEDATA_v2.H
# End Source File
# Begin Source File

SOURCE=.\InternalStateData.cpp
# End Source File
# Begin Source File

SOURCE=.\InternalStateData.h
# End Source File
# Begin Source File

SOURCE=.\midimap_v2.cpp
# End Source File
# Begin Source File

SOURCE=.\midimap_v2.h
# End Source File
# Begin Source File

SOURCE=.\MyConsts.h
# End Source File
# Begin Source File

SOURCE=.\MyHelpers.cpp
# End Source File
# Begin Source File

SOURCE=.\MyHelpers.h
# End Source File
# Begin Source File

SOURCE=.\Presets.cpp
# End Source File
# Begin Source File

SOURCE=.\Presets.h
# End Source File
# Begin Source File

SOURCE=.\Wav.cpp
# End Source File
# Begin Source File

SOURCE=.\Wav.h
# End Source File
# End Group
# Begin Group "VstGui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Banner.cpp
# End Source File
# Begin Source File

SOURCE=.\Banner.h
# End Source File
# Begin Source File

SOURCE=.\CheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckBox.h
# End Source File
# Begin Source File

SOURCE=.\ControlPositioners.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlPositioners.h
# End Source File
# Begin Source File

SOURCE=.\Editor.cpp
# End Source File
# Begin Source File

SOURCE=.\Editor.hpp
# End Source File
# Begin Source File

SOURCE=.\Envelope.cpp
# End Source File
# Begin Source File

SOURCE=.\Envelope.h
# End Source File
# Begin Source File

SOURCE=.\IntAdjuster.cpp
# End Source File
# Begin Source File

SOURCE=.\IntAdjuster.h
# End Source File
# Begin Source File

SOURCE=.\IOSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\IOSlider.h
# End Source File
# Begin Source File

SOURCE=.\Knob.cpp
# End Source File
# Begin Source File

SOURCE=.\Knob.h
# End Source File
# Begin Source File

SOURCE=.\LevelSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\LevelSlider.h
# End Source File
# Begin Source File

SOURCE=.\LightUpBox.cpp
# End Source File
# Begin Source File

SOURCE=.\LightUpBox.h
# End Source File
# Begin Source File

SOURCE=.\MixerChannelEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\MixerChannelEditor.h
# End Source File
# Begin Source File

SOURCE=.\MixerOutline.cpp
# End Source File
# Begin Source File

SOURCE=.\MixerOutline.h
# End Source File
# Begin Source File

SOURCE=.\MixSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\MixSlider.h
# End Source File
# Begin Source File

SOURCE=.\MyOptionMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\MyOptionMenu.h
# End Source File
# Begin Source File

SOURCE=.\PatternEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\PatternEditor.h
# End Source File
# Begin Source File

SOURCE=.\PopupNameEdit.CPP
# End Source File
# Begin Source File

SOURCE=.\PopupNameEdit.H
# End Source File
# Begin Source File

SOURCE=.\ShuffleSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\ShuffleSlider.h
# End Source File
# Begin Source File

SOURCE=.\SleepingReporter.cpp
# End Source File
# Begin Source File

SOURCE=.\SleepingReporter.h
# End Source File
# Begin Source File

SOURCE=.\SoundEditPane.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundEditPane.h
# End Source File
# Begin Source File

SOURCE=.\SoundNameEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundNameEdit.h
# End Source File
# Begin Source File

SOURCE=.\SpinButton.cpp
# End Source File
# Begin Source File

SOURCE=.\SpinButton.h
# End Source File
# End Group
# Begin Group "WinGui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\About.cpp
# End Source File
# Begin Source File

SOURCE=.\About.h
# End Source File
# Begin Source File

SOURCE=.\ContextMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\ContextMenu.h
# End Source File
# Begin Source File

SOURCE=.\Help.cpp
# End Source File
# Begin Source File

SOURCE=.\Help.h
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.h
# End Source File
# End Group
# End Target
# End Project
