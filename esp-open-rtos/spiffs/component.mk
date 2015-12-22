# Component makefile

INC_DIRS += $(spiffs_ROOT)

# args for passing into compile rule generation
spiffs_SRC_DIR =  $(spiffs_ROOT)

$(eval $(call component_compile_rules,spiffs))