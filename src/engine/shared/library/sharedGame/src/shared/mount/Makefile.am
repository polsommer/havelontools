AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libsharedGamesharedmount.la
libsharedGamesharedmount_la_CXXFLAGS=-I$(srcdir)/../../../include/public \
	-I$(srcdir)/../../../include/private \
	-I$(engine_dir)/shared/library/sharedCompression/include/public \
	-I$(engine_dir)/shared/library/sharedDebug/include/public \
	-I$(engine_dir)/shared/library/sharedFile/include/public \
	-I$(engine_dir)/shared/library/sharedFoundation/include/public \
	-I$(engine_dir)/shared/library/sharedFoundationTypes/include/public \
	-I$(engine_dir)/shared/library/sharedLog/include/public \
	-I$(engine_dir)/shared/library/sharedMath/include/public \
	-I$(engine_dir)/shared/library/sharedMathArchive/include/public \
	-I$(engine_dir)/shared/library/sharedMessageDispatch/include/public \
	-I$(engine_dir)/shared/library/sharedMemoryManager/include/public \
	-I$(engine_dir)/shared/library/sharedNetworkMessages/include/public \
	-I$(engine_dir)/shared/library/sharedObject/include/public \
	-I$(engine_dir)/shared/library/sharedRandom/include/public \
	-I$(engine_dir)/shared/library/sharedSynchronization/include/public \
	-I$(engine_dir)/shared/library/sharedSkillSystem/include/public \
	-I$(engine_dir)/shared/library/sharedThread/include/public \
	-I$(engine_dir)/shared/library/sharedUtility/include/public \
	-I$(stlport_dir)/stlport \
	-I$(boost_dir) \
	-I$(udplibrary_dir) \
	-I$(archive_dir)/include \
	-I$(unicode_dir)/include \
	-I$(unicodeArchive_dir)/include/public \
	-I$(localization_dir)/include \
	-I$(localizationArchive_dir)/include/public \
	-I$(fileInterface_dir)/include/public \
	-Wno-ctor-dtor-privacy
libsharedGamesharedmount_la_SOURCES=MountValidScaleRangeTable.cpp \
	MountValidScaleRangeTable.h \
	SharedSaddleManager.cpp \
	SharedSaddleManager.h

libsharedGamesharedmount_la_LDFLAGS=-avoid-version

include $(engine_dir)/../make_include/engine.include
