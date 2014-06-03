#######################################################################################################################
#######################################################################################################################
### HAL9001 Rubix Cube Solving Robot - gen_init_state.py
### Chris Gerth - Summer 2014
###
### Description: Rotates the cube and takes pictures of all six faces. Does basic image recognition on each face to
###              determine the initial state of the cube.
###
#######################################################################################################################
#######################################################################################################################

from PIL import Image
import os, math

#color defs
CubeColor_undef = -1
CubeColor_red = 0
CubeColor_blue = 1
CubeColor_green = 2
CubeColor_yellow = 3
CubeColor_orange = 4
CubeColor_white = 5



#given an acquisition stage, acq_cube_rotate puts the cube in the proper position to be 
#   photographed by the pi camera.
def acq_cube_rotate(stage):
    if stage == 0: #init from load position, prep to  image front
        print "Moving to image front face"
#ADD SERVO MOVING CODE HERE
    elif stage == 1: #prep to image right
        print "Moving to image right face"
#ADD SERVO MOVING CODE HERE
    elif stage == 2: #prep to image back
        print "Moving to image back face"
#ADD SERVO MOVING CODE HERE
    elif stage == 3: #prep to image left
        print "Moving to image left face"
#ADD SERVO MOVING CODE HERE
    elif stage == 4: #prep to image top
        print "Moving to image top face"
#ADD SERVO MOVING CODE HERE
    elif stage == 5: #prep to image bottom
        print "Moving to image bottom face"
#ADD SERVO MOVING CODE HERE
    elif stage == 6: #return to home(?) position
        print "Moving to prep for solving"
#ADD SERVO MOVING CODE HERE

 
    

#Generates an image for each side of the cube
def acquire_imgs():
    for stage_iter in range(0,6):
        acq_cube_rotate(stage_iter); #properly position cube
        cmd = "raspistill -o temp/face"+str(stage_iter)+".jpg -n" #generate command
        os.system(cmd) #take photo
    acq_cube_rotate(6); #return cube to home position

    
    
#######################################################################################################################
#Start of main function (gen_init_state.py)
#######################################################################################################################

#define pixel locations in cube face images TWEAK THESE TO MATCH REAL-WORLD DATA
top_row_pxl = 80
mid_row_pxl = 126 
bot_row_pxl = 172
lft_col_pxl = 50
mid_col_pxl = 94
rgt_col_pxl = 140

#define colors RGB Vals TWEAK THESE TO MATCH REAL-WORLD DATA
red_r = 159
red_g = 40
red_b = 0
blue_r = 0
blue_g = 59
blue_b = 159
green_r = 0
green_g = 127
green_b = 86
yellow_r = 235
yellow_g = 205
yellow_b = 23
orange_r = 200
orange_g = 100
orange_b = 20
white_r = 214
white_g = 255
white_b = 244

#predefine distances
dist_red = 0
dist_blue = 0
dist_green = 0
dist_yellow = 0
dist_orange = 0
dist_white = 0

#define test pixel location vectors. tell the algorithm which pixel to use for color determination for each cubie face
pxl_locs = [[(lft_col_pxl, top_row_pxl),(mid_col_pxl, top_row_pxl),(rgt_col_pxl, top_row_pxl)],
            [(lft_col_pxl, mid_row_pxl),(mid_col_pxl, mid_row_pxl),(rgt_col_pxl, mid_row_pxl)],
            [(lft_col_pxl, bot_row_pxl),(mid_col_pxl, bot_row_pxl),(rgt_col_pxl, bot_row_pxl)]]

#create struct for storing the cube colors
cube_colors = [[CubeColor_undef]*9 for i in range(6)]

#get six images for the Rubik's cube
acquire_imgs() #COMMENT IS TEMP/DEBUG

#open each image, fill the cube_colors array with the selected color
for img_iter in range(0, 6):
    img_path = "../temp/face" + str(img_iter) + ".jpg"
    im = Image.open(img_path)
    rgb_im = im.convert('RGB')
    for x_iter in range(0,3): #iterate over all nine color locations on each face
        for y_iter in range(0,3):
            r, g, b = rgb_im.getpixel(pxl_locs[y_iter][x_iter]) #get pixel value
            #find euclidian distances
            dist_red = math.sqrt( math.pow(r - red_r, 2) + math.pow(g - red_g, 2) + math.pow(b - red_b, 2))
            dist_blue = math.sqrt( math.pow(r - blue_r, 2) + math.pow(g - blue_g, 2) + math.pow(b - blue_b, 2))
            dist_green = math.sqrt( math.pow(r - green_r, 2) + math.pow(g - green_g, 2) + math.pow(b - green_b, 2))
            dist_yellow = math.sqrt( math.pow(r - yellow_r, 2) + math.pow(g - yellow_g, 2) + math.pow(b - yellow_b, 2))
            dist_orange = math.sqrt( math.pow(r - orange_r, 2) + math.pow(g - orange_g, 2) + math.pow(b - orange_b, 2))
            dist_white = math.sqrt( math.pow(r - white_r, 2) + math.pow(g - white_g, 2) + math.pow(b - white_b, 2))
            dist_min = min(dist_red, dist_blue, dist_green, dist_yellow, dist_orange, dist_white) #find minimum distance value
            print(dist_min)
            #figure out which color has that minimum value
            if(dist_min == dist_red):
                color = CubeColor_red
            elif(dist_min == dist_green):
                color = CubeColor_green
            elif(dist_min == dist_blue):
                color = CubeColor_blue
            elif(dist_min == dist_yellow):
                color = CubeColor_yellow
            elif(dist_min == dist_orange):
                color = CubeColor_orange
            else:
                color = CubeColor_white
            #set cubie face as that color
            cube_colors[img_iter][3*y_iter + x_iter] = color
            
print cube_colors # debug
# add code here to calculate the cubie positions based off of a lookup table
# and generate a string representing the initial state of the cubie.
