from tkinter import *
import tkinter.messagebox
import customtkinter

customtkinter.set_appearance_mode("System")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("green")  # Themes: "blue" (standard), "green", "dark-blue"


class App(customtkinter.CTk):

    

    def __init__(self):
        super().__init__()
        # configure window
        self.title("Leather Lamination Program Demo  - by IERC Team")
        self.geometry(f"{1100}x{580}")

        # configure grid layout (4x4)
        self.grid_columnconfigure((0), weight=0)
        self.grid_columnconfigure((1), weight=3)
        self.grid_rowconfigure((0, 1, 2,3), weight=1)

        # create sidebar frame with widgets
        self.sidebar_frame = customtkinter.CTkFrame(self, width=140, height=1000, corner_radius=0)
        self.sidebar_frame.grid(row=0, column=0, rowspan=4, sticky="nsew")
        self.sidebar_frame.grid_rowconfigure(4, weight=1)
        self.logo_label = customtkinter.CTkLabel(self.sidebar_frame, text="Leather Lamination Program ", font=customtkinter.CTkFont(size=20, weight="bold"))
        self.logo_label.grid(row=0, column=0, padx=20, pady=(20, 10))
        self.label = customtkinter.CTkLabel(self.sidebar_frame, text="                                                  -by IERC Team ", font=customtkinter.CTkFont(size=10))
        self.label.grid(row=1, column=0, padx=5, pady=(5, 10))
        self.sidebar_button_1 = customtkinter.CTkButton(self.sidebar_frame, text= "START", command=self.sidebar_button_event)
        self.sidebar_button_1.grid(row=2, column=0, padx=20, pady=10)
        self.sidebar_button_2 = customtkinter.CTkButton(self.sidebar_frame, text="STOP", hover_color= "#FF0000", command=self.sidebar_button_event)
        self.sidebar_button_2.grid(row=3, column=0, padx=20, pady=10)
        
        self.appearance_mode_label = customtkinter.CTkLabel(self.sidebar_frame, text="Appearance Mode:", anchor="w")
        self.appearance_mode_label.grid(row=5, column=0, padx=20, pady=(10, 0))
        self.appearance_mode_optionemenu = customtkinter.CTkOptionMenu(self.sidebar_frame, values=["Light", "Dark", "System"],
                                                                       command=self.change_appearance_mode_event)
        self.appearance_mode_optionemenu.grid(row=6, column=0, padx=20, pady=(10, 10))
        self.scaling_label = customtkinter.CTkLabel(self.sidebar_frame, text="Zoomn +/- :", anchor="w")
        self.scaling_label.grid(row=7, column=0, padx=20, pady=(10, 0))
        self.scaling_optionemenu = customtkinter.CTkOptionMenu(self.sidebar_frame, values=["80%", "90%", "100%", "110%", "120%"],
                                                               command=self.change_scaling_event)
        self.scaling_optionemenu.grid(row=8, column=0, padx=20, pady=(10, 20))

        # create main entry and button
        self.info_frame = customtkinter.CTkScrollableFrame(self, fg_color="transparent")
        self.info_frame.grid(row=0, column=1, padx=(20, 20),rowspan=6,  pady=(20,20), sticky="nsew")
        self.info_frame.grid_columnconfigure((0,1,2,3), weight=0)
        self.info_frame.grid_rowconfigure((0), weight=1)
        
        self.Upper_label = customtkinter.CTkLabel(self.info_frame, text="Upper Jaw Temperature :",font=customtkinter.CTkFont(family="Calibri",size=20, weight="bold"), anchor="w")
        self.Upper_label.grid(row=0, column=0, columnspan=3, padx=20, pady=(20,5), sticky='w')
        self.ts1_label = customtkinter.CTkLabel(self.info_frame, text="Set Temperature :", anchor="w")
        self.ts1_label.grid(row=1, column=0, padx=20, pady=(10, 10))
        self.entry = customtkinter.CTkEntry(self.info_frame, placeholder_text="Temperature in celcius")
        self.entry.grid(row=1, column=1, padx=(20, 40), pady=(20,20), sticky="w")

        self.tp1_label = customtkinter.CTkLabel(self.info_frame, text="Present Temperature :", anchor="w")
        self.tp1_label.grid(row=1, column=3, padx=20, pady=(10, 10))
        self.tp1_label = customtkinter.CTkLabel(self.info_frame, text="28", anchor="w")
        self.tp1_label.grid(row=1, column=4, padx=20, pady=(10, 10))

        self.Lower_label = customtkinter.CTkLabel(self.info_frame, text="Lower Jaw Temperature :",font=customtkinter.CTkFont(family="Calibri",size=20, weight="bold"), anchor="w")
        self.Lower_label.grid(row=3, column=0, columnspan=3, padx=20, pady=(20,5), sticky='w')
        self.ts2_label = customtkinter.CTkLabel(self.info_frame, text="Set Temperature :", anchor="w")
        self.ts2_label.grid(row=4, column=0, padx=20, pady=(10, 10))
        self.entry = customtkinter.CTkEntry(self.info_frame, placeholder_text="Temperature in celcius")
        self.entry.grid(row=4, column=1, padx=(20, 40), pady=(20, 20), sticky="w")

        self.tp2_label = customtkinter.CTkLabel(self.info_frame, text="Present Temperature :", anchor="w")
        self.tp2_label.grid(row=4, column=3, padx=20, pady=(10, 10))
        self.tp2_label = customtkinter.CTkLabel(self.info_frame, text="28", anchor="w")
        self.tp2_label.grid(row=4, column=4, padx=20, pady=(10, 10))

        self.Time_label = customtkinter.CTkLabel(self.info_frame, text="Time :",font=customtkinter.CTkFont(family="Calibri",size=20, weight="bold"), anchor="w")
        self.Time_label.grid(row=5, column=0, columnspan=3, padx=20, pady=(20,5), sticky='w')
        self.t1_label = customtkinter.CTkLabel(self.info_frame, text="PreSet :", anchor="w")
        self.t1_label.grid(row=6, column=0, padx=20, pady=(10, 10), sticky='w')
        self.entry = customtkinter.CTkEntry(self.info_frame, placeholder_text="Time in sec")
        self.entry.grid(row=6, column=1, padx=(20, 40), pady=(20, 20), sticky="w")

        self.t2_label = customtkinter.CTkLabel(self.info_frame, text="Set :", anchor="w")
        self.t2_label.grid(row=6, column=3, padx=20, pady=(10, 10), sticky='w')
        self.t2_label = customtkinter.CTkLabel(self.info_frame, text="5", anchor="w")
        self.t2_label.grid(row=6, column=4, padx=20, pady=(10, 10))






       
        
        # set default values
        
        
        self.appearance_mode_optionemenu.set("System")
       
    def open_input_dialog_event(self):
        dialog = customtkinter.CTkInputDialog(text="Type in a number:", title="CTkInputDialog")
        print("CTkInputDialog:", dialog.get_input())

    def change_appearance_mode_event(self, new_appearance_mode: str):
        customtkinter.set_appearance_mode(new_appearance_mode)

    def change_scaling_event(self, new_scaling: str):
        new_scaling_float = int(new_scaling.replace("%", "")) / 100
        customtkinter.set_widget_scaling(new_scaling_float)

    def sidebar_button_event(self):
        print("sidebar_button click")


if __name__ == "__main__":
    app = App()
    app.mainloop()