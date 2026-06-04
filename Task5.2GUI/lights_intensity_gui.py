from gpiozero import PWMLED, LED
import tkinter as tk


living_room = PWMLED(17)
bathroom = LED(27)
closet = LED(22)

def set_brightness(value):

    living_room.value = float(value) / 100

def toggle_bathroom():
    if bathroom_on.get() == 1:
        bathroom.on()
    else:
        bathroom.off()

def toggle_closet():
    if closet_on.get() == 1:
        closet.on()
    else:
        closet.off()

def exit_program():
    living_room.off()
    bathroom.off()
    closet.off()
    window.destroy()

window = tk.Tk()
window.title("Linda's Light Control")
window.geometry("340x340")

title = tk.Label(window, text="Living Room Light Intensity", font=("Arial", 14))
title.pack(pady=10)

brightness = tk.Scale(
    window,
    from_=0,
    to=100,
    orient="horizontal",
    length=250,
    label="Brightness (%)",
    command=set_brightness,
)
brightness.pack(pady=10)

bathroom_on = tk.IntVar()
closet_on = tk.IntVar()

bathroom_box = tk.Checkbutton(window, text="Bathroom light", font=("Arial", 12),
                              variable=bathroom_on, command=toggle_bathroom)
bathroom_box.pack(anchor="w", padx=50)

closet_box = tk.Checkbutton(window, text="Closet light", font=("Arial", 12),
                            variable=closet_on, command=toggle_closet)
closet_box.pack(anchor="w", padx=50)

exit_button = tk.Button(window, text="Exit", font=("Arial", 12), command=exit_program)
exit_button.pack(pady=20)

window.mainloop()
