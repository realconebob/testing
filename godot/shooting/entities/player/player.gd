extends BaseEntity

func _ready() -> void:
	set_max_speed(12000)

func _physics_process(delta: float) -> void:
	var dir := Input.get_vector(&"move_left", &"move_right", &"move_up", &"move_down")
	set_acc(dir * 8000)
	super(delta)
