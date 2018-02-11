import bge
from bge import texture
from bge import logic

def init():
    # To get the controller thats running this python script:
    cont = logic.getCurrentController()
    # To get the game object this controller is on:
    obj = cont.owner

    # Identify a static texture by name
    matID = texture.materialID(obj, 'IMwebcam.jpg')

    # Create a dynamic texture that will replace the static texture
    logic.camera = texture.Texture(obj, matID)

    # Define a source of image for the texture, here a movie
    logic.camera.source = texture.VideoFFmpeg('/dev/video0', 0, 25, 640, 480)
    logic.camera.source.scale = True

    logic.camera.source.play()

def main():
    logic.camera.refresh(True)

if not hasattr(logic, 'camera'):
    init()

main()