##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=MyMap
ConfigurationName      :=Debug
WorkspacePath          :=/home/dmitriy/MyEnv/cprog/CodeLiteWorkspaces/test
ProjectPath            :=/home/dmitriy/MyEnv/cprog/CodeLiteWorkspaces/test/MyMap
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Dmitriy
Date                   :=14/05/16
CodeLitePath           :=/home/dmitriy/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="MyMap.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -lgcov  --coverage
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall  --coverage  -std=c++11  -w $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall  $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/MyCPPMap.cpp$(ObjectSuffix) $(IntermediateDirectory)/MyCPPMapTest.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/MyCPPMap.cpp$(ObjectSuffix): MyCPPMap.cpp $(IntermediateDirectory)/MyCPPMap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitriy/MyEnv/cprog/CodeLiteWorkspaces/test/MyMap/MyCPPMap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MyCPPMap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MyCPPMap.cpp$(DependSuffix): MyCPPMap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MyCPPMap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MyCPPMap.cpp$(DependSuffix) -MM "MyCPPMap.cpp"

$(IntermediateDirectory)/MyCPPMap.cpp$(PreprocessSuffix): MyCPPMap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MyCPPMap.cpp$(PreprocessSuffix) "MyCPPMap.cpp"

$(IntermediateDirectory)/MyCPPMapTest.cpp$(ObjectSuffix): MyCPPMapTest.cpp $(IntermediateDirectory)/MyCPPMapTest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dmitriy/MyEnv/cprog/CodeLiteWorkspaces/test/MyMap/MyCPPMapTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MyCPPMapTest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MyCPPMapTest.cpp$(DependSuffix): MyCPPMapTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MyCPPMapTest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MyCPPMapTest.cpp$(DependSuffix) -MM "MyCPPMapTest.cpp"

$(IntermediateDirectory)/MyCPPMapTest.cpp$(PreprocessSuffix): MyCPPMapTest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MyCPPMapTest.cpp$(PreprocessSuffix) "MyCPPMapTest.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


