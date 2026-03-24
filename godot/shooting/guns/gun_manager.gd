class_name GunManager
extends Node2D

const available_gun_scenes := [
	preload("res://guns/pistol/pistol.tscn"),
	preload("res://guns/shotgun/shotgun.tscn")
]

var enabled_guns: Array[BaseGun] = [
	available_gun_scenes[0].instantiate()
]

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var pointing: Line2D = Line2D.new()
	pointing.clear_points()
	pointing.add_point(Vector2.ZERO)
	pointing.add_point(Vector2(150, 0))
	pointing.default_color = Color(1, 0, 0)
	
	add_child(pointing)
	pass # Replace with function body.

func _input(event: InputEvent) -> void:
	return

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
