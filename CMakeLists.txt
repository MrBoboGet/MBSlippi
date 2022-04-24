project(MBSlippi)
set(PROJECT_NAME "MBSlippi")
##BEGIN MBPM_VARIABLES
set(MBPM_DEPENDENCIES 
	"MrBoboDatabase"
	"sqlite3"
	"MBUtility"
	"MBUnicode"
	"MBScript"
	"MBParsing"
	"MBPacketManager"
	"MBCLI"
	"MBCrypto"
	"cryptopp"
	"MrBigInt"
	"MrBoboSockets"
	"MBMime"
)
set(MBPM_SystemLibraries
Ws2_32.lib
Secur32.lib
Bcrypt.lib
Mfplat.lib
opengl32
Mfuuid.lib
	Strmiids.lib
)
set(MBPM_DependanciesIncludeDirectories
)
set(MBPM_TARGET_EXTPACKET_LIBRARIES )
set(MBPM_TARGET_COMPILE_OPTIONS )
set(MBPM_TARGET_LINK_OPTIONS )
#MBPM_Functions#variables MBPM_DEPENDENCIES
#MBPM_TARGET_EXTPACKET_LIBRARIES
#MBPM_TARGET_COMPILE_OPTIONS
#MBPM_TARGET_LINK_OPTIONS
#Environment variables MBPM_PACKETS_INSTALL_DIRECTORY
set(MBPM_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(MBPM_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
set(MBPM_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
macro(MBPM_UpdateLibraries TargetToUpdate LinkType BuildType)
	set(MBPM_TARGET_EXTPACKET_LIBRARIES "")
	foreach(PACKET IN LISTS MBPM_DEPENDENCIES)
		set(NewLibraryName "")
		set(PacketName ${PACKET})
		set(LibraryName ${PACKET})
		if(${PACKET} MATCHES "#")
			string(REPLACE "#" ";" PACKET_PART_LIST ${PACKET})
			list(GET PACKET_PART_LIST 0 PacketName)
			list(GET PACKET_PART_LIST 1 LibraryName)
		endif()
		set(NewLibraryToAdd "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/${PacketName}/MBPM_Builds/")
		if(${LinkType} STREQUAL "STATIC")
			set(NewLibraryName "${LibraryName}_S")
		elseif(${LinkType} STREQUAL "DYNAMIC")
			set(NewLibraryName "${LibraryName}_D")
		endif()
		if(${BuildType} STREQUAL "DEBUG")
			set(NewLibraryName "${NewLibraryName}D")
		elseif(${BuildType} STREQUAL "RELEASE")
			set(NewLibraryName "${NewLibraryName}R")
		endif()
		if(UNIX)
			set(NewLibraryName "lib${NewLibraryName}.")
			if(${LinkType} STREQUAL "STATIC")
				set(NewLibraryName "${NewLibraryName}a")
			else()
				set(NewLibraryName "${NewLibraryName}so")
			endif()
		else()
			if(${LinkType} STREQUAL "STATIC")
				set(NewLibraryName "${NewLibraryName}.lib")
			else()
				set(NewLibraryName "${NewLibraryName}.dll")				
			endif()
		endif()
		set(NewLibraryToAdd "${NewLibraryToAdd}${NewLibraryName}")
		list(APPEND MBPM_TARGET_EXTPACKET_LIBRARIES ${NewLibraryToAdd})
	endforeach()
endmacro()

macro(MBPM_UpdateTargetVariables TargetToUpdate LinkType BuildType)
	MBPM_UpdateLibraries(${TargetToUpdate} ${LinkType} ${BuildType})
endmacro()

macro(MBPM_UpdateOutput TargetToUpdate)
	set_target_properties(${TargetToUpdate} 
	PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	)
	#set_target_properties(${TargetToUpdate} PROPERTIES PREFIX "")
	#set_target_properties(${TargetToUpdate} PROPERTIES SUFFIX "")
	set_target_properties(${TargetToUpdate} PROPERTIES OUTPUT_NAME ${TargetToUpdate})
endmacro()

macro(MBPM_SetTargetCompileOptions TargetToUpdate LinkType BuildType)
	set(CMAKE_CXX_FLAGS " ")
	set(CMAKE_CXX_FLAGS_DEBUG " ")
	set(CMAKE_CXX_FLAGS_RELEASE " ")
	set(CMAKE_C_FLAGS " ")
	set(CMAKE_C_FLAGS_DEBUG " ")
	set(CMAKE_C_FLAGS_RELEASE " ")

	if(${BuildType} STREQUAL "DEBUG" AND ${LinkType} STREQUAL "STATIC")
		set(NEW_TARGET_COMPILE_OPTIONS "${MBPM_CXX_FLAGS} ${MBPM_CXX_FLAGS_DEBUG}")
	else()
		set(NEW_TARGET_COMPILE_OPTIONS "${MBPM_CXX_FLAGS} ${MBPM_CXX_FLAGS_RELEASE}")
	endif()
	string(REPLACE " " ";" NEW_TARGET_COMPILE_OPTIONS_LIST ${NEW_TARGET_COMPILE_OPTIONS})
	target_compile_options(${TargetToUpdate} PRIVATE ${NEW_TARGET_COMPILE_OPTIONS_LIST})
	message(${TargetToUpdate} " Compiled with follwing flags: ""${NEW_TARGET_COMPILE_OPTIONS_LIST}")
endmacro()

macro(MBPM_LinkTargetPacketLibraries Target)
	target_link_libraries(${Target} ${MBPM_TARGET_EXTPACKET_LIBRARIES})
	target_link_libraries(${Target} ${MBPM_SystemLibraries})
endmacro()

macro(MBPM_SetTargetIncludeDirectories Target)
	target_include_directories(${Target} PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/"
	)
	target_include_directories(${Target} PUBLIC ${MBPM_DependanciesIncludeDirectories})
endmacro()

macro(MBPM_ApplyTargetConfiguration TargetToUpdate LinkType BuildType)
	MBPM_UpdateTargetVariables(${TargetToUpdate} ${LinkType} ${BuildType})
	MBPM_UpdateOutput(${TargetToUpdate})
	MBPM_SetTargetCompileOptions(${TargetToUpdate} ${LinkType} ${BuildType})
	MBPM_SetTargetIncludeDirectories(${TargetToUpdate})
endmacro()
##END MBPM_VARIABLES
set(PROJECT_SOURCES 

	"${CMAKE_CURRENT_SOURCE_DIR}/MBSlippi.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBSlippiParsing.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBSlippiTypes.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBSlippiScript.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBSlippi_CLI.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBSlippiConfig.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBMeleeID.cpp"
)
set(PROJECT_HEADERS 
	"${CMAKE_CURRENT_SOURCE_DIR}//MBSlippi.h"
)

set(EXECUTABLE_FILES
	${PROJECT_SOURCES}
	"${CMAKE_CURRENT_SOURCE_DIR}/MBSlippi_CLIMain.cpp"
)

set(COMMON_FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS})
set(COMMON_DYNAMIC_LIBRARIES 
)
set(COMMON_STATIC_LIBRARIES 
)
set(COMMON_LIBRARIES ${COMMON_STATIC_LIBRARIES} ${COMMON_DYNAMIC_LIBRARIES})

add_library(MBSlippi_SD STATIC ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBSlippi_SD" "STATIC" "DEBUG")
target_compile_features("MBSlippi_SD" PRIVATE cxx_std_17)
target_link_libraries("MBSlippi_SD" ${COMMON_LIBRARIES})

add_library(MBSlippi_SR STATIC ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBSlippi_SR" "STATIC" "RELEASE")
target_compile_features("MBSlippi_SR" PRIVATE cxx_std_17)
target_link_libraries("MBSlippi_SR" ${COMMON_LIBRARIES})



add_executable("mbslippitest" ${EXECUTABLE_FILES})
MBPM_ApplyTargetConfiguration("mbslippitest" "STATIC" "RELEASE")
target_compile_features("mbslippitest" PRIVATE cxx_std_17)
MBPM_LinkTargetPacketLibraries("mbslippitest")
