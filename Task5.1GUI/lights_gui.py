from gpiozero import LED
import tkinter as tk

living_room = LED(17)
bathroom = LED(27)
closet = LED(22)

lights = {
    "Living Room": living_room,
    "Bathroom": bathroom,
    "Closet": closet,
}

def update_lights():
    selected = choice.get()
    for name, led in lights.items():
        if name == selected:
            led.on()
        else:
            led.off()

def exit_program():
    for led in lights.values():
        led.off()
    window.destroy()

window = tk.Tk()
window.title("Linda's Light Switch")
window.geometry("320x280")

choice = tk.StringVar(value="")

title = tk.Label(window, text="Choose a room to light up", font=("Arial", 14))
title.pack(pady=15)

for name in lights:
    button = tk.Radiobutton(
        window,
        text=name,
        variable=choice,
        value=name,
        font=("Arial", 12),
        command=update_lights,
    )
    button.pack(anchor="w", padx=50)

exit_button = tk.Button(window, text="Exit", font=("Arial", 12), command=exit_program)
exit_button.pack(pady=20)

window.mainloop()
