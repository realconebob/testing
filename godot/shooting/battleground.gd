extends Node2D

func _physics_process(delta: float) -> void:
	$Zombie.set_target($Player.position)
