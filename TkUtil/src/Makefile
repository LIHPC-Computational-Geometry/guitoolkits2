#-----------------------------------------------------------------------------#
# ATTENTION: Ce makefile ne s'execute qu'avec GNU make ('gmake').             #
# La variable MAKE doit etre positionnee sur gmake, et un alias sur make doit #
# egalement pointer vers gmake.                                               #
#-----------------------------------------------------------------------------#

# Makefile du module tkutil. Produit LIB_UTIL.


# ============================================================================
#                         VARIABLES A PARAMETRER
# ============================================================================

# Compilateurs C++ :
# ==================

# Optionnel, voir determination automatique ci-apres.
# Par defaut (si non specifie) le compilateur C++ natif est pris. On peut
# forcer ici l'utilisation de g++, ou forcer l'utilisation de g++ a la ligne de
# commande comme suit :
# 	make CPP_COMPILER=G++
# Valeurs possibles : rien, G++, SUN_CC, OSF_CC, SGI_CC
CPP_COMPILER	:= 


# Mode de production :
# ====================

# Valeurs possibles : DEBUG
#MODE			:= DEBUG


# ============================================================================
#                             PLATE-FORME
# ============================================================================

include ../config/plate-forme.mk


# ============================================================================
#             DETERMINATION AUTOMATIQUE DES COMPILATEURS
# ============================================================================

include ../config/compilers_determination.mk


CONFIG	:= CEA_OS=$(CEA_OS) CPP_COMPILER=$(CPP_COMPILER)
CONFIG	+= MODE=$(MODE)


# ============================================================================
#                            ORGANISATION PROJET
# ============================================================================

include ../config/organization.mk

MODULE_DIR		:= tkutil/
MODULE_VERSION	:= $(UTIL_VERSION)


# ============================================================================
#                            COMPILATEURS
# ============================================================================

include ../config/compilers.mk
include ../config/modules.mk


# ============================================================================
#                                    CIBLE ALL
# ============================================================================


PUBLISHER	:= 1

.PHONY:all
all:swig shlib lib exe


SWIG_SRC	:= $(PYTHON_DIR)UtilPythonBinding.cc

.PHONY:swig
swig:$(SWIG_SRC)

MODULE_LIBS			:= $(LIB_UTIL)
MODULE_SHLIB		:= $(SH_LIB_UTIL)
MODULE_SHLIB_SHORT	:= $(SH_LIB_UTIL_NAME)
MODULE_SHLIB_LINK	:= $(SH_LIB_UTIL_LINK_NAME)

.PHONY:lib
lib:$(MODULE_LIBS)
PUBLISHED_HEADERS	:= $(INC_DIR)*.*
PUBLISHED_PY_FILES	:= $(PYTHON_DIR)$(PY_UTIL_MODULE_NAME).pyc
PUBLISHED_PY_FILES	+= $(LIB_DIR)_$(PY_UTIL_MODULE_NAME).so

.PHONY:shlib
shlib:swig $(MODULE_SHLIB)

.PHONY:exe
exe:
	cd tests; $(MAKE) $(CONFIG) all;

.PHONY:publish_exe
publish_exe : ;


# ============================================================================
#                         DRAPEAUX DE COMPILATION
# ============================================================================

C++FLAGS		+= -D$(LM_TARGET) -DUTIL_VERSION='"$(UTIL_VERSION)"'
C++FLAGS		+= $(SHARED_OBJECTS_FLAGS)


# ============================================================================
#                               SOURCES 
# ============================================================================

# Listes des fichiers C et C++ :
# ==============================

C++_SRC	:= $(wildcard $(SRC_DIR)*.cc)


# Fichiers objets :
# =================

OBJS	:= $(subst $(SRC_DIR),$(OBJ_DIR), $(subst .cc,.o, $(C++_SRC)))


# ============================================================================
#                                    CIBLES
# ============================================================================


DOXYGEN			:= doxygen
DOXYGEN_CFG		:= doc/doxygen.cfg


.PHONY:clean
clean:
	\rm -f $(LIB_UTIL) $(SH_LIB_UTIL) $(OBJS) $(OBJ_DIR)UtilPythonBinding.o
	\rm -f $(PY_UTIL_MODULE) $(LIB_DIR)$(SH_LIB_UTIL_NAME)
	\rm -f $(LIB_DIR)$(SH_LIB_UTIL_LINK_NAME)
	\rm -f $(PYTHON_DIR)$(PY_UTIL_MODULE_NAME).p*
	\rm -f $(PYTHON_DIR)UtilPythonBinding.cc
	\rm -rf $(REPOSITORIES)
	\rm -rf $(SWIG_SRC)
	cd tests; $(MAKE) $(CONFIG) clean;

.PHONY:cleanall
cleanall:clean
	\rm -f $(OBJ_DIR).depend
	\rm -rf $(API_DOC_DIR)
	cd tests; $(MAKE) $(CONFIG) cleanall;

.PHONY:config
config:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(PYTHON_DIR)
	cd tests; $(MAKE) $(CONFIG) config;

.PHONY:doc
doc:
	\rm -rf $(API_DOC_DIR)
	mkdir -p $(API_DOC_DIR)
	$(DOXYGEN) $(DOXYGEN_CFG)
#	cd tests; $(MAKE) doc;

-include $(OBJ_DIR).depend
include ../config/libraries.mk
include ../config/c_rules.mk
include ../config/targets.mk


# ============================================================================
#                           REGLES DE TRANSFORMATION
# ============================================================================


ALL_INCLUDES	= -I$(INC_DIR)
#ifneq ($(USE_PYTHON),)
ALL_INCLUDES	+= $(PYTHON_INC)
#endif	# ifneq ($(USE_PYTHON),)


# .o -> $(LIB_UTIL) :
# ===================

$(LIB_UTIL):$(OBJS)
	@echo
	@echo "==> Création de $@ ..."
	$(AR) $@ $^ $(TMPL_OBJS)
	@echo "==> Création de $@ effectuée"

# .o -> $(SH_LIB_UTIL) :
# ======================

SWIG_FLAGS	+= -module $(PY_UTIL_MODULE_NAME)
SWIG_FLAGS	+= -globals $(PY_UTIL_MODULE_NAME)
SWIG_FLAGS	+= -I$(INC_DIR)

$(SH_LIB_UTIL):$(OBJS) $(OBJ_DIR)UtilPythonBinding.o
	@echo
	@echo "==> Création de $@ ..."
	$(AR_DL) $(AR_DL_NAME)$(SH_LIB_UTIL_NAME) $(AR_DL_FLAGS) $@ $^ $(TMPL_OBJS)\
	         $(RPATH_OPT)$(C++_RT_LIB_PATH) $(C++_RT_LIB)
	cp $(SH_LIB_UTIL) $(PY_UTIL_MODULE)
	cd $(LIB_DIR); rm -f $(SH_LIB_UTIL_NAME) $(SH_LIB_UTIL_LINK_NAME);    \
	   ln -s $(SH_VER_LIB_UTIL_NAME) $(SH_LIB_UTIL_NAME);                 \
	   ln -s $(SH_LIB_UTIL_NAME) $(SH_LIB_UTIL_LINK_NAME)
	PYTHONPATH=$(dir $(SH_LIB_UTIL)):$(PYTHON_DIR); export PYTHONPATH;    \
	$(SWIG_DIR)compile_binding.py
	@echo "==> Création de $@ effectuée"


