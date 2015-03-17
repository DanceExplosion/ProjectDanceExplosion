
Graphics3D 800, 600

MoveMouse 400,300

pivot=CreatePivot()
PositionEntity pivot,0,1.5,0

camera=CreateCamera( pivot )
CameraClsColor camera,100,100,100

PositionEntity camera,0,50,-100


AmbientLight 200, 200, 200


light3=CreateLight()
LightColor light3,180,120,220
RotateEntity light3,0,90,0
PositionEntity light3, 40,20,0


light3=CreateLight()
LightColor light3,160,120,30
RotateEntity light3,0,0,0
PositionEntity light3, -40,20,0



dude=LoadAnimMesh( "ant01.b3d" )
;dude=LoadAnimMesh( "ant02.b3d" )



plane=CreatePlane()
PositionEntity plane, 0,.01,0
EntityAlpha plane,.5
mirror=CreateMirror()



;ExtractAnimSeq dude, 1, 9	;walk
;ExtractAnimSeq dude, 10, 19 ;attack
;ExtractAnimSeq dude, 20, 29 ;stun
;ExtractAnimSeq dude, 31, 45 ;explode
;ExtractAnimSeq dude, 47, 65 ;idle


ExtractAnimSeq dude, 1, 65 ;all animations



seq=1



While Not KeyHit(1)


If Not Animating( dude )
		Animate dude,1,.2,seq,10
		
	
	EndIf


If KeyDown( 203 )=True Then x#=x#-0.5
If KeyDown( 205 )=True Then x#=x#+0.5
If KeyDown( 208 )=True Then y#=y#-0.5
If KeyDown( 200 )=True Then y#=y#+0.5

If KeyDown( 205 )=True Then MoveEntity camera,1,0,0
If KeyDown( 203 )=True Then MoveEntity camera,-1,0,0
If KeyDown( 208 )=True Then MoveEntity camera,0,0,-1
If KeyDown( 200 )=True Then MoveEntity camera,0,0,1

x#=EntityX(camera)
y#=EntityY(camera)
z#=EntityZ(camera)
	

	
	Mouselook(camera)
;mouselook routine call

PositionEntity camera,x#,y#,z#

	
	UpdateWorld
	RenderWorld
	
	Text 0,0,"Mouse steers camera, cursors move camera"
	Flip
	
	If KeyHit(59) Then
SaveBuffer(FrontBuffer(),"grab"+ss+".bmp"):ss=ss+1
EndIf
;Press F1 For a screenshot
	
	
	
	
Wend

End




;Control
; Camera position, angle values
Global cam_x#,cam_z#,cam_pitch#,cam_yaw#,cam_speed#=.5		; Current
Global dest_cam_x#,dest_cam_z#,dest_cam_pitch#,dest_cam_yaw#	; Destination
Global ent_x#,ent_z#,ent_pitch#,ent_yaw#,ent_speed#=.5		; Current
Global dest_ent_x#,dest_ent_z#,dest_ent_pitch#,dest_ent_yaw#	; Destination

Function mouselook(camera)
	; Mouse look
	; ----------

	; Mouse x and y speed
	mxs=MouseXSpeed()
	mys=MouseYSpeed()
	
	; Mouse shake (total mouse movement)
	mouse_shake=Abs(((mxs+mys)/2)/1000.0)

	; Destination camera angle x and y values
	dest_cam_yaw#=dest_cam_yaw#-mxs
	dest_cam_pitch#=dest_cam_pitch#+mys

	; Current camera angle x and y values
	cam_yaw=cam_yaw+((dest_cam_yaw-cam_yaw)/5)
	cam_pitch=cam_pitch+((dest_cam_pitch-cam_pitch)/5)
	
	RotateEntity camera,cam_pitch#,cam_yaw#,0
	;RotateEntity camera,mxs,mys,0
		
	; Rest mouse position to centre of screen
	MoveMouse 400,300

		; Move camera using movement values
	MoveEntity camera,x#,y#,z#
		
End Function	






FreeEntity dude