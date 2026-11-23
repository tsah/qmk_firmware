SRC += tsah.c
CAPS_WORD_ENABLE = yes

# Home row mod recording build (see users/tsah/hrm_log/README.md):
#   qmk compile -kb keebart/3w6_pro -km tsah -e TSAH_HRM_LOG=yes
ifeq ($(strip $(TSAH_HRM_LOG)), yes)
    CONSOLE_ENABLE = yes
    SRC += hrm_log.c
    OPT_DEFS += -DTSAH_HRM_LOG
endif
