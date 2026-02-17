extends Node2D

@onready var screen_size: Vector2 = get_viewport().get_visible_rect().size
@onready var mspeed: float = sqrt((screen_size.x ** 2) + (screen_size.y ** 2)) / 5

func _physics_process(delta: float) -> void:
	var guy: Guy = $Guy
	var heading: Vector2 = (get_global_mouse_position() - guy.position).normalized()
	guy.global_rotation = heading.angle()
	guy.global_position += mspeed * heading * delta
	
	return


func _on_cursor_follower_area_entered(area: Area2D) -> void:
	if area == $Guy/Area2D:
		var newx: float = randf_range(0, screen_size.x)
		var newy: float = randf_range(0, screen_size.y)
		$Guy.position = Vector2(newx, newy)
	
	return
