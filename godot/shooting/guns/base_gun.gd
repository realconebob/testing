class_name BaseGun
extends Node2D

signal fired(bullets: Array[BaseEntity])

var target: Vector2 = Vector2.ZERO
var bpos: Vector2 = Vector2.ZERO

func fire() -> void:
	fired.emit(make_bullets(target, bpos))
	return

func set_target(t: Vector2) -> void:
	target = t
	
func get_target() -> Vector2:
	return target

func set_bullet_position(pos: Vector2) -> void:
	bpos = pos
	
func get_bullet_position() -> Vector2:
	return bpos

func make_bullets(_target: Vector2, _position: Vector2) -> Array[BaseEntity]:
	assert(false, "<BaseGun::make_bullets> Error: This function is to be overwritten, not called")
	return []

func get_handle() -> Node2D:
	assert(false, "<BaseGun::get_handle> Error: This function is to be overwritten, not called")
	return null
	
func get_barrel() -> Node2D:
	assert(false, "<BaseGun::get_barrel> Error: This function is to be overwritten, not called")
	return null
