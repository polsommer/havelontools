AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=TemplateDefinitionCompiler
TemplateDefinitionCompiler_SOURCES=FirstTemplateDefinitionCompiler.cpp
TemplateDefinitionCompiler_CXXFLAGS=-I$(engine_dir)/shared/library/sharedFoundation/include/public \
	-I$(engine_dir)/shared/library/sharedFoundationTypes/include/public \
	-I$(engine_dir)/shared/library/sharedMemoryManager/include/public \
	-I$(engine_dir)/shared/library/sharedDebug/include/public \
	-I$(stlport_dir)/stlport \
	-Wno-ctor-dtor-privacy
TemplateDefinitionCompiler_LDADD=$(top_builddir)/../engine/shared/library/sharedMemoryManager/libsharedMemoryManager.la \
	../shared/libTemplateDefinitionCompilershared.la \
	$(top_builddir)/../engine/shared/library/sharedCompression/libsharedCompression.la \
	$(top_builddir)/../engine/shared/library/sharedDebug/libsharedDebug.la \
	$(top_builddir)/../engine/shared/library/sharedFile/libsharedFile.la \
	$(top_builddir)/../engine/shared/library/sharedMath/libsharedMath.la \
	$(top_builddir)/../engine/shared/library/sharedRandom/libsharedRandom.la \
	$(top_builddir)/../engine/shared/library/sharedRegex/libsharedRegex.la \
	$(top_builddir)/../engine/shared/library/sharedSynchronization/libsharedSynchronization.la \
	$(top_builddir)/../engine/shared/library/sharedTemplate/libsharedTemplate.la \
	$(top_builddir)/../engine/shared/library/sharedTemplateDefinition/libsharedTemplateDefinition.la \
	$(top_builddir)/../engine/shared/library/sharedThread/libsharedThread.la \
	$(top_builddir)/../engine/shared/library/sharedUtility/libsharedUtility.la \
	$(top_builddir)/../engine/shared/library/sharedXml/libsharedXml.la \
	$(top_builddir)/../engine/shared/library/sharedFoundation/libsharedFoundation.la \
	$(top_builddir)/../external/ours/library/archive/libarchive.la \
	$(top_builddir)/../external/ours/library/fileInterface/libfileInterface.la \
	$(top_builddir)/../external/ours/library/localization/liblocalization.la \
	$(top_builddir)/../external/ours/library/unicode/libunicode.la \
	$(top_builddir)/../external/ours/library/unicodeArchive/libunicodeArchive.la \
	$(libxml_dir)/libxml2.la \
	$(stlport_dir)/lib/libstlport_gcc.a \
	$(pcre_dir)/lib/libpcre.a \
	$(perforce_dir)/lib/$(platform)/libclient.a \
	$(perforce_dir)/lib/$(platform)/librpc.a \
	$(perforce_dir)/lib/$(platform)/libsupp.a
	
TemplateDefinitionCompiler_LDFLAGS=-lpthread -ldl -lm -lncurses -lz

#
#	$(perforce_dir)/lib/$(platform)/libclient.a \
#	$(perforce_dir)/lib/$(platform)/librpc.a \
#	$(perforce_dir)/lib/$(platform)/libsupp.a \
#

../shared/libTemplateDefinitionCompilershared.la:
	make -C ../shared

$(top_builddir)/../engine/shared/library/sharedCompression/libsharedCompression.la:
	make -C $(top_builddir)/../engine/shared/library/sharedCompression

$(top_builddir)/../engine/shared/library/sharedDebug/libsharedDebug.la:
	make -C $(top_builddir)/../engine/shared/library/sharedDebug

$(top_builddir)/../engine/shared/library/sharedFile/libsharedFile.la:
	make -C $(top_builddir)/../engine/shared/library/sharedFile

$(top_builddir)/../engine/shared/library/sharedFoundation/libsharedFoundation.la:
	make -C $(top_builddir)/../engine/shared/library/sharedFoundation

$(top_builddir)/../engine/shared/library/sharedGame/libsharedGame.la:
	make -C $(top_builddir)/../engine/shared/library/sharedGame

$(top_builddir)/../engine/shared/library/sharedLog/libsharedLog.la:
	make -C $(top_builddir)/../engine/shared/library/sharedLog

$(top_builddir)/../engine/shared/library/sharedMath/libsharedMath.la:
	make -C $(top_builddir)/../engine/shared/library/sharedMath

$(top_builddir)/../engine/shared/library/sharedMemoryManager/libsharedMemoryManager.la:
	make -C $(top_builddir)/../engine/shared/library/sharedMemoryManager

$(top_builddir)/../engine/shared/library/sharedRandom/libsharedRandom.la:
	make -C $(top_builddir)/../engine/shared/library/sharedRandom

$(top_builddir)/../engine/shared/library/sharedRegex/libsharedRegex.la:
	make -C $(top_builddir)/../engine/shared/library/sharedRegex

$(top_builddir)/../engine/shared/library/sharedSynchronization/libsharedSynchronization.la:
	make -C $(top_builddir)/../engine/shared/library/sharedSynchronization

$(top_builddir)/../engine/shared/library/sharedTemplate/libsharedTemplate.la:
	make -C $(top_builddir)/../engine/shared/library/sharedTemplate

$(top_builddir)/../engine/shared/library/sharedTemplateDefinition/libsharedTemplateDefinition.la:
	make -C $(top_builddir)/../engine/shared/library/sharedTemplateDefinition

$(top_builddir)/../engine/shared/library/sharedThread/libsharedThread.la:
	make -C $(top_builddir)/../engine/shared/library/sharedThread

$(top_builddir)/../engine/shared/library/sharedUtility/libsharedUtility.la:
	make -C $(top_builddir)/../engine/shared/library/sharedUtility

$(top_builddir)/../engine/shared/library/sharedXml/libsharedXml.la:
	make -C $(top_builddir)/../engine/shared/library/sharedXml

$(top_builddir)/../external/ours/library/archive/libarchive.la:
	make -C $(top_builddir)/../external/ours/library/archive

$(top_builddir)/../external/ours/library/fileInterface/libfileInterface.la:
	make -C $(top_builddir)/../external/ours/library/fileInterface

$(top_builddir)/../external/ours/library/localization/liblocalization.la:
	make -C $(top_builddir)/../external/ours/library/localization

$(top_builddir)/../external/ours/library/unicode/libunicode.la:
	make -C $(top_builddir)/../external/ours/library/unicode

$(top_builddir)/../external/ours/library/unicodeArchive/libunicodeArchive.la:
	make -C $(top_builddir)/../external/ours/library/unicodeArchive

$(stlport_dir)/lib/libstlport_gcc.a:
	make -C $(stlport_dir)/src -f gcc-linux.mak


include $(engine_dir)/../make_include/engine.include
