locals @@

.186
.model tiny

.code

;==========================================================================
; Draws an animated snowflake on the screen
; Entry: ES:DI - screen address
;	 AL    - letter in the center
; Destr:
;==========================================================================

snowflake	proc
		
		