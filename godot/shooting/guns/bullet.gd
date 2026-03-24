extends BaseEntity

func _ready() -> void:
	super()
	$VisibleOnScreenNotifier2D.screen_exited.connect(_handle_offscreen)
	
func _handle_offscreen() -> void:
	queue_free()
