# Build the sketch folder for the environment (wro_car_ackermann, wro_car_diff, huskylens_arduino)
import os
Import("env")
env.Replace(PROJECT_SRC_DIR=os.path.join(env["PROJECT_DIR"], env.GetProjectOption("custom_sketch", env["PIOENV"])))
