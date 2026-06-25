import pyttsx3
import tkinter as tk
from tkinter import messagebox
import smtplib
from email.message import EmailMessage
from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Spacer, Paragraph, HRFlowable
from reportlab.lib.pagesizes import A4
from reportlab.lib import colors
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import mm
from reportlab.lib.enums import TA_CENTER, TA_LEFT
import os
import subprocess
import threading
import math
import random
import datetime

# ===================== SETTINGS =====================
CPP_EXE    = r"C:\Users\3tee\OneDrive\Desktop\G4(6)\library_management.exe"
USERS_FILE = r"C:\Users\3tee\OneDrive\Desktop\G4(6)\users.txt"
SENDER_EMAIL = ""
APP_PASSWORD = ""
# ====================================================

try:
    engine = pyttsx3.init()
except Exception:
    engine = None

def speak(text):
    try:
        if engine:
            engine.say(text)
            engine.runAndWait()
    except Exception:
        pass

# ===================== COLOR PALETTE =====================
BG          = "#080812"
CARD_BG     = "#0f0f1e"
ACCENT      = "#00e5ff"
ACCENT2     = "#7c4dff"
ACCENT3     = "#ff6b6b"
TEXT        = "#e8eaf6"
MUTED       = "#5c6bc0"
SUCCESS     = "#00e676"
ERROR_COLOR = "#ff1744"
BORDER      = "#1e1e3a"
INPUT_BG    = "#12122a"
HOVER_BTN   = "#00b8d4"
GOLD        = "#ffd700"

# Chat colors
CHAT_BG         = "#0a0a18"
CHAT_SIDEBAR_BG = "#07070f"
CHAT_MSG_USER   = "#1a1a35"
CHAT_MSG_BOT    = "#0f1829"
CHAT_INPUT_BG   = "#111128"
CHAT_ACCENT     = "#00e5ff"
CHAT_ACCENT2    = "#7c4dff"
CHAT_TEXT       = "#e8eaf6"
CHAT_MUTED      = "#4a5080"
CHAT_BORDER     = "#1a1a30"
# =========================================================

# ===================== CHATBOT BRAIN =====================
CHATBOT_RESPONSES = {
    "harry potter":   ["Harry Potter by J.K. Rowling — ID: 1 | Category: Fantasy. A classic!"],
    "alchemist":      ["The Alchemist by Paulo Coelho — ID: 2 | Category: Fiction. Highly recommended!"],
    "clean code":     ["Clean Code by Robert C. Martin — ID: 3 | Category: Programming. A must-read for devs!"],
    "algorithms":     ["Introduction to Algorithms by Cormen et al. — ID: 4 | Category: Programming."],
    "pride":          ["Pride and Prejudice by Jane Austen — ID: 5 | Category: Classic. Timeless!"],
    "fantasy":        ["Fantasy section: Harry Potter (ID: 1). Ask the librarian to add more fantasy books!"],
    "programming":    ["Programming books: Clean Code (ID: 3) & Intro to Algorithms (ID: 4). Both excellent!"],
    "fiction":        ["Fiction: The Alchemist (ID: 2) by Paulo Coelho. A journey of self-discovery!"],
    "classic":        ["Classics: Pride and Prejudice (ID: 5). Jane Austen at her finest!"],
    "available":      ["5 books available:\n  1. Harry Potter (Fantasy)\n  2. The Alchemist (Fiction)\n  3. Clean Code (Programming)\n  4. Intro to Algorithms (Programming)\n  5. Pride & Prejudice (Classic)"],
    "books":          ["We have 5 default books across Fantasy, Fiction, Programming & Classic genres!"],
    "recommend":      ["Recommendations:\n  • For devs → Clean Code (ID:3)\n  • For fun → Harry Potter (ID:1)\n  • For wisdom → The Alchemist (ID:2)"],
    "suggest":        ["Suggestions based on mood:\n  • Adventurous → Harry Potter\n  • Philosophical → The Alchemist\n  • Technical → Clean Code"],
    "borrow limit":   ["Borrow limit is 2 books per user at a time!"],
    "how many":       ["You can borrow up to 2 books at a time (Max_borrow_limit = 2 in system)"],
    "borrow":         ["To borrow: login → choose option 3 → enter Book ID.\nLimit: 2 books at a time!"],
    "return":         ["Return within 7 days to avoid fines!\nGo to login → option 4 → enter Book ID."],
    "due":            ["Due date = 7 days from borrow date. Return before that to avoid Rs.10/day fine!"],
    "fine":           ["Fine = Rs. 10 per day after 7-day due date.\nEven 1 hour late = charged for that day!"],
    "late":           ["Late fee: Rs. 10/day after due date. Even 1 hour counts as a full day!"],
    "defaulter":      ["If fine > your balance → you become a DEFAULTER!\nPay remaining amount at the counter."],
    "fee":            ["Late fee = Rs. 10/day after 7 days. Fine deducted from your Rs.1000 balance."],
    "balance":        ["Every new user starts with Rs. 1,000.00 balance.\nFines are deducted from this!"],
    "money":          ["Starting balance: Rs. 1,000. Overdue fines reduce this. Keep returning on time!"],
    "register":       ["Click REGISTER NEW USER on the home screen!\nOr use option [1] in the terminal."],
    "login":          ["Use option [2] in the terminal with your username & password to login."],
    "password":       ["Password is set during registration. It is case-sensitive — keep it safe!"],
    "username":       ["Username is chosen during registration. Used to login and track your borrows."],
    "admin":          ["Librarian Panel = option [3] in terminal.\nAdmin can add/delete books & view all records."],
    "librarian":      ["Librarian powers:\n  • Add new books\n  • Delete books (if not borrowed)\n  • View all users\n  • View all borrow records"],
    "add book":       ["Only the Librarian can add books via terminal option [3] → Admin Panel → [1] Add Resource."],
    "delete":         ["Librarian can delete books — BUT only if the book is NOT currently borrowed!"],
    "email":          ["Welcome email + PDF report sent automatically on registration to your email!"],
    "pdf":            ["A personalized PDF with your member profile is generated & emailed on signup!"],
    "system":         ["CIS SHELF = Python GUI (registration) + C++ terminal (library operations).\nTwo-part system!"],
    "id":             ["Book IDs:\n  1=Harry Potter\n  2=Alchemist\n  3=Clean Code\n  4=Algorithms\n  5=Pride&Prejudice"],
    "category":       ["Categories: Fantasy, Fiction, Programming, Classic.\nAsk librarian to add more!"],
    "help":           ["I can help with:\n  • Book recommendations\n  • Borrow & return rules\n  • Fine & balance info\n  • Registration & login help\n  • System info"],
    "hello":          ["Hello! Welcome to CIS SHELF AI Assistant!\nAsk me anything about the library!",
                       "Hey there! I am your CIS SHELF assistant. What do you need?"],
    "hi":             ["Hi! How can I help you today?",
                       "Hello! Ask me about books, fines, or borrowing rules!"],
    "thanks":         ["You are welcome! Happy Reading!",
                       "Anytime! Come back if you need help!"],
    "bye":            ["Goodbye! Visit CIS SHELF again soon!"],
    "who made":       ["CIS SHELF was built by Daniyal Ahmed — Group 4, 2025!"],
    "who created":    ["Created by Daniyal Ahmed. Python GUI + C++ backend — impressive combo!"],
    "about":          ["CIS SHELF — Digital Library Management System\nBy Daniyal Ahmed | Group 4 | 2025\nPython + C++ powered!"],
}

DEFAULT_RESPONSES = [
    "Try: 'available books', 'borrow limit', or 'what is the fine'",
    "I know books, fines, balance & rules. Try: 'recommend a book'!",
    "Not sure about that! Ask: 'how many books can I borrow?' or 'suggest programming books'",
    "Hint: Ask about specific books, fines, balance, or borrowing rules!",
]

def get_bot_response(text):
    try:
        text_lower = text.lower().strip()
        for keyword, responses in CHATBOT_RESPONSES.items():
            if keyword in text_lower:
                return random.choice(responses)
        return random.choice(DEFAULT_RESPONSES)
    except Exception:
        return "Sorry, I had a small glitch! Try asking again."
# =========================================================


def save_user_to_file(username, password, firstname, lastname, address, email):
    with open(USERS_FILE, "a") as f:
        f.write(f"{username}|{password}|{firstname}|{lastname}|{address}|{email}|1000.0|0\n")


def generate_pdf(username, firstname, lastname, address, email):
    pdf_file = r"C:\Users\3tee\OneDrive\Desktop\G4(6)\\" + username + "_report.pdf"
    doc = SimpleDocTemplate(
        pdf_file, pagesize=A4,
        leftMargin=20*mm, rightMargin=20*mm,
        topMargin=15*mm, bottomMargin=15*mm
    )
    styles = getSampleStyleSheet()
    title_style = ParagraphStyle('TitleStyle', parent=styles['Title'], fontSize=28,
        textColor=colors.HexColor("#00e5ff"), spaceAfter=2, fontName='Helvetica-Bold',
        alignment=TA_CENTER, leading=34)
    sub_style = ParagraphStyle('SubStyle', parent=styles['Normal'], fontSize=11,
        textColor=colors.HexColor("#7c4dff"), spaceAfter=6, fontName='Helvetica-Bold', alignment=TA_CENTER)
    tagline_style = ParagraphStyle('TaglineStyle', parent=styles['Normal'], fontSize=9,
        textColor=colors.HexColor("#5c6bc0"), spaceAfter=4, fontName='Helvetica', alignment=TA_CENTER)
    label_style = ParagraphStyle('LabelStyle', parent=styles['Normal'], fontSize=9,
        textColor=colors.HexColor("#5c6bc0"), fontName='Helvetica-Bold', alignment=TA_LEFT)
    value_style = ParagraphStyle('ValueStyle', parent=styles['Normal'], fontSize=11,
        textColor=colors.white, fontName='Helvetica', alignment=TA_LEFT)
    welcome_style = ParagraphStyle('WelcomeStyle', parent=styles['Normal'], fontSize=11,
        textColor=colors.HexColor("#e8eaf6"), fontName='Helvetica', alignment=TA_CENTER, leading=18)
    footer_style = ParagraphStyle('FooterStyle', parent=styles['Normal'], fontSize=8,
        textColor=colors.HexColor("#1e1e3a"), fontName='Helvetica', alignment=TA_CENTER)
    story = []
    header_data = [[Paragraph("CIS SHELF", title_style)]]
    header_table = Table(header_data, colWidths=[170*mm])
    header_table.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(-1,-1),colors.HexColor("#0a0a1a")),
        ("TOPPADDING",(0,0),(-1,-1),18),("BOTTOMPADDING",(0,0),(-1,-1),6),
        ("LEFTPADDING",(0,0),(-1,-1),0),("RIGHTPADDING",(0,0),(-1,-1),0),
        ("LINEBELOW",(0,0),(-1,-1),2,colors.HexColor("#00e5ff")),
    ]))
    story.append(header_table)
    story.append(Spacer(1,3*mm))
    story.append(Paragraph("DIGITAL LIBRARY MANAGEMENT SYSTEM", sub_style))
    story.append(Paragraph("Your Gateway to Knowledge — Since 2025", tagline_style))
    story.append(Spacer(1,4*mm))
    story.append(HRFlowable(width="100%",thickness=1,color=colors.HexColor("#7c4dff"),spaceAfter=4*mm,spaceBefore=0))
    story.append(Paragraph(
        f"Welcome aboard, <b>{firstname} {lastname}</b>!<br/>"
        "Your library membership has been successfully created.<br/>"
        "Below are your registration details for your records.", welcome_style))
    story.append(Spacer(1,5*mm))
    def make_row(label, value, icon=""):
        return [Paragraph(f"{icon}  {label}", label_style), Paragraph(str(value), value_style)]
    reg_data = [
        [Paragraph("MEMBER PROFILE", ParagraphStyle('hdr', parent=styles['Normal'],
            fontSize=10, textColor=colors.HexColor("#00e5ff"), fontName='Helvetica-Bold', alignment=TA_LEFT)),
         Paragraph("", label_style)],
        make_row("USERNAME", username, ""), make_row("FIRST NAME", firstname, ""),
        make_row("LAST NAME", lastname, ""), make_row("ADDRESS", address, ""),
        make_row("EMAIL", email, ""), make_row("BALANCE", "Rs. 1,000.00", ""),
        make_row("STATUS", "ACTIVE", ""),
    ]
    reg_table = Table(reg_data, colWidths=[55*mm, 115*mm])
    reg_table.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(-1,0),colors.HexColor("#0d0d22")),("SPAN",(0,0),(-1,0)),
        ("TOPPADDING",(0,0),(-1,0),10),("BOTTOMPADDING",(0,0),(-1,0),10),
        ("LINEBELOW",(0,0),(-1,0),1.5,colors.HexColor("#00e5ff")),
        ("ROWBACKGROUNDS",(0,1),(-1,-1),[colors.HexColor("#12122a"),colors.HexColor("#0f0f1e")]),
        ("TEXTCOLOR",(0,1),(-1,-1),colors.white),
        ("TOPPADDING",(0,1),(-1,-1),9),("BOTTOMPADDING",(0,1),(-1,-1),9),
        ("LEFTPADDING",(0,0),(-1,-1),12),("RIGHTPADDING",(0,0),(-1,-1),12),
        ("LINEBELOW",(0,1),(-1,-2),0.5,colors.HexColor("#1e1e3a")),
        ("LINEBELOW",(0,-1),(-1,-1),2,colors.HexColor("#7c4dff")),
        ("BOX",(0,0),(-1,-1),1.5,colors.HexColor("#00e5ff")),
        ("VALIGN",(0,0),(-1,-1),"MIDDLE"),
    ]))
    story.append(reg_table)
    story.append(Spacer(1,5*mm))
    balance_data = [[
        Paragraph("STARTING BALANCE", ParagraphStyle('bal_lbl', parent=styles['Normal'],
            fontSize=9, textColor=colors.HexColor("#5c6bc0"), fontName='Helvetica-Bold')),
        Paragraph("Rs. 1,000.00", ParagraphStyle('bal_val', parent=styles['Normal'],
            fontSize=20, textColor=colors.HexColor("#ffd700"), fontName='Helvetica-Bold', alignment=TA_LEFT)),
    ]]
    balance_table = Table(balance_data, colWidths=[60*mm, 110*mm])
    balance_table.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(-1,-1),colors.HexColor("#0d0d22")),
        ("TOPPADDING",(0,0),(-1,-1),14),("BOTTOMPADDING",(0,0),(-1,-1),14),
        ("LEFTPADDING",(0,0),(-1,-1),14),("RIGHTPADDING",(0,0),(-1,-1),14),
        ("BOX",(0,0),(-1,-1),1.5,colors.HexColor("#ffd700")),
        ("VALIGN",(0,0),(-1,-1),"MIDDLE"),
    ]))
    story.append(balance_table)
    story.append(Spacer(1,5*mm))
    tips_data = [[Paragraph(
        "<b>Getting Started Tips</b><br/>"
        "  Log in with your username and password to access the system.<br/>"
        "  Browse thousands of books across all genres.<br/>"
        "  Your balance will be used for borrowing premium titles.<br/>"
        "  Return books on time to avoid late fees.",
        ParagraphStyle('tips', parent=styles['Normal'], fontSize=9,
            textColor=colors.HexColor("#b0bec5"), fontName='Helvetica', leading=16))]]
    tips_table = Table(tips_data, colWidths=[170*mm])
    tips_table.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(-1,-1),colors.HexColor("#0a1a0a")),
        ("TOPPADDING",(0,0),(-1,-1),12),("BOTTOMPADDING",(0,0),(-1,-1),12),
        ("LEFTPADDING",(0,0),(-1,-1),14),("RIGHTPADDING",(0,0),(-1,-1),14),
        ("BOX",(0,0),(-1,-1),1,colors.HexColor("#00e676")),
        ("LINEAFTER",(0,0),(0,-1),4,colors.HexColor("#00e676")),
    ]))
    story.append(tips_table)
    story.append(Spacer(1,5*mm))
    story.append(HRFlowable(width="100%",thickness=1,color=colors.HexColor("#1e1e3a"),spaceAfter=3*mm))
    story.append(Paragraph(
        "2025 CIS SHELF — E-Library Management System  |  Daniyal Ahmed<br/>"
        "This document is auto-generated. Please keep it for your records.", footer_style))
    doc.build(story)
    return pdf_file


def send_email_func(pdf_file, firstname, email_receiver):
    msg = EmailMessage()
    msg["Subject"] = "CIS SHELF — Welcome, Your Library Card is Ready!"
    msg["From"]    = SENDER_EMAIL
    msg["To"]      = email_receiver
    body = f"""Hello {firstname},

Welcome to CIS SHELF — your digital library, reimagined!

Your registration has been completed successfully. We are thrilled to have you as a member of our growing reading community.

YOUR MEMBERSHIP IS ACTIVE
Starting Balance: Rs. 1,000.00
Status: ACTIVE

Your personalized registration report is attached to this email as a PDF. Please keep it for your records.

To get started:
  1. Open the CIS SHELF application
  2. Log in with your username and password
  3. Explore thousands of books at your fingertips!

If you have any questions, feel free to reach out.

Happy Reading!
The CIS SHELF Team

2025 CIS SHELF | Daniyal Ahmed
"""
    msg.set_content(body)
    with open(pdf_file, "rb") as f:
        msg.add_attachment(f.read(), maintype="application", subtype="pdf",
                           filename=os.path.basename(pdf_file))
    with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
        server.login(SENDER_EMAIL, APP_PASSWORD)
        server.send_message(msg)


# ===================== ANIMATED BORDER BUTTON =====================
class BorderAnimButton(tk.Canvas):
    def __init__(self, master, text="", command=None,
                 btn_bg=CARD_BG, btn_fg=ACCENT,
                 font_spec=("Courier New", 11, "bold"),
                 height=46, width=400,
                 border_color=ACCENT, **kwargs):
        super().__init__(master, bg=BG, highlightthickness=0, height=height, width=width)
        self.command      = command
        self.btn_bg       = btn_bg
        self.btn_fg       = btn_fg
        self.border_color = border_color
        self.text_str     = text
        self.font_spec    = font_spec
        self.w            = width
        self.h            = height
        self._angle       = 0
        self._animating   = False
        self._hover       = False
        self._draw()
        self.bind("<Enter>",          self._on_enter)
        self.bind("<Leave>",          self._on_leave)
        self.bind("<Button-1>",       self._on_click)
        self.bind("<ButtonRelease-1>",self._on_release)

    def _draw(self):
        self.delete("all")
        w, h = self.w, self.h
        pad = 2
        self.create_rectangle(pad, pad, w-pad, h-pad, fill=self.btn_bg, outline="", tags="bg")
        self.create_rectangle(pad, pad, w-pad, h-pad, fill="", outline=BORDER, width=1, tags="border_dim")
        if self._hover or self._animating:
            self._draw_glow_border()
        fg = self.btn_fg if not self._hover else "#ffffff"
        self.create_text(w//2, h//2, text=self.text_str, fill=fg, font=self.font_spec, tags="label")

    def _draw_glow_border(self):
        w, h = self.w, self.h
        pad  = 2
        perimeter = 2*(w+h) - 8*pad
        a    = (self._angle % 360) / 360
        seg  = perimeter * 0.25
        start = a * perimeter
        def pos_on_rect(t):
            t = t % perimeter
            if t < w-2*pad:   return pad+t, pad
            t -= (w-2*pad)
            if t < h-2*pad:   return w-pad, pad+t
            t -= (h-2*pad)
            if t < w-2*pad:   return w-pad-t, h-pad
            t -= (w-2*pad)
            return pad, h-pad-t
        steps = 30
        points = [pos_on_rect(start + seg*i/steps) for i in range(steps+1)]
        if len(points) >= 2:
            for i in range(len(points)-1):
                alpha = i/(len(points)-1)
                c = self._interpolate_color(self.border_color, "#001a20", alpha)
                x1,y1 = points[i]; x2,y2 = points[i+1]
                self.create_line(x1,y1,x2,y2,fill=c,width=2,capstyle="round")

    def _interpolate_color(self, c1, c2, t):
        r1,g1,b1 = int(c1[1:3],16),int(c1[3:5],16),int(c1[5:7],16)
        r2,g2,b2 = int(c2[1:3],16),int(c2[3:5],16),int(c2[5:7],16)
        return f"#{int(r1*(1-t)+r2*t):02x}{int(g1*(1-t)+g2*t):02x}{int(b1*(1-t)+b2*t):02x}"

    def _on_enter(self, e):
        self._hover = True
        if not self._animating:
            self._animating = True
            self._animate()

    def _on_leave(self, e):
        self._hover = False

    def _animate(self):
        if not self._hover and not self._animating:
            return
        self._angle = (self._angle + 6) % 360
        self._draw()
        if self._hover:
            self.after(16, self._animate)
        else:
            self._animating = False
            self._draw()

    def _on_click(self, e):
        self.config(bg="#050510")

    def _on_release(self, e):
        self.config(bg=BG)
        if self.command:
            self.command()

    def set_text(self, t):
        self.text_str = t
        self._draw()

    def set_state(self, state):
        if state == "disabled":
            self.btn_fg = MUTED
            self.unbind("<Button-1>")
            self.unbind("<ButtonRelease-1>")
        else:
            self.btn_fg = ACCENT
            self.bind("<Button-1>",        self._on_click)
            self.bind("<ButtonRelease-1>", self._on_release)
        self._draw()


# ===================== INLINE CHAT PAGE =====================
class ChatPage(tk.Frame):
    """
    Full-screen ChatGPT-style chat interface embedded inside the main window.
    Opens by replacing the current frame, has its own Exit button to go back.
    """
    def __init__(self, master):
        super().__init__(master, bg=CHAT_BG)
        self._chat_history = []   # list of (role, text, timestamp)
        self._typing       = False
        self._build()
        self._post_welcome()

    # ── Layout ──────────────────────────────────────────────────────────────
    def _build(self):
        # ── Top header bar ──────────────────────────────────────
        header = tk.Frame(self, bg="#06060f", height=56)
        header.pack(fill="x")
        header.pack_propagate(False)

        # Left: back arrow + title
        left = tk.Frame(header, bg="#06060f")
        left.pack(side="left", fill="y", padx=14)

        back_btn = tk.Canvas(left, bg="#06060f", highlightthickness=0, width=32, height=32, cursor="hand2")
        back_btn.pack(side="left", pady=12)
        back_btn.create_polygon(20,8, 8,16, 20,24, fill=ACCENT, outline="")
        back_btn.bind("<Button-1>", lambda e: self.master.show_welcome())
        back_btn.bind("<Enter>", lambda e: back_btn.itemconfig(1, fill="#ffffff"))
        back_btn.bind("<Leave>", lambda e: back_btn.itemconfig(1, fill=ACCENT))

        tk.Label(left, text="CIS SHELF  ASSISTANT",
                 font=("Courier New", 13, "bold"), bg="#06060f", fg=ACCENT).pack(side="left", padx=(10,0))

        # Right: status dot + exit button
        right = tk.Frame(header, bg="#06060f")
        right.pack(side="right", fill="y", padx=14)

        # Status
        self._dot_on = True
        self.dot_lbl = tk.Label(right, text="  LIVE", font=("Courier New", 8, "bold"),
                                bg="#06060f", fg=SUCCESS)
        self.dot_lbl.pack(side="right", pady=20)
        self._blink_dot()

        # Exit button — visible, prominent
        exit_btn = tk.Button(
            right, text="  EXIT CHAT",
            font=("Courier New", 9, "bold"),
            bg=ERROR_COLOR, fg="white",
            relief="flat", cursor="hand2",
            padx=14, pady=6,
            activebackground="#cc0000", activeforeground="white",
            command=self.master.show_welcome
        )
        exit_btn.pack(side="right", pady=12, padx=(0, 8))

        # Accent line under header
        tk.Frame(self, bg=ACCENT2, height=2).pack(fill="x")

        # ── Body: sidebar + chat ─────────────────────────────────
        body = tk.Frame(self, bg=CHAT_BG)
        body.pack(fill="both", expand=True)

        # Sidebar
        sidebar = tk.Frame(body, bg=CHAT_SIDEBAR_BG, width=170)
        sidebar.pack(side="left", fill="y")
        sidebar.pack_propagate(False)
        self._build_sidebar(sidebar)

        # Vertical separator
        tk.Frame(body, bg=CHAT_BORDER, width=1).pack(side="left", fill="y")

        # Main chat area
        chat_area = tk.Frame(body, bg=CHAT_BG)
        chat_area.pack(side="left", fill="both", expand=True)
        self._build_chat_area(chat_area)

    def _build_sidebar(self, parent):
        tk.Label(parent, text="CONVERSATIONS",
                 font=("Courier New", 7, "bold"), bg=CHAT_SIDEBAR_BG, fg=CHAT_MUTED,
                 anchor="w").pack(fill="x", padx=12, pady=(14, 6))

        tk.Frame(parent, bg=CHAT_BORDER, height=1).pack(fill="x", padx=12)

        # Current session item
        session_frame = tk.Frame(parent, bg="#12122a", cursor="hand2")
        session_frame.pack(fill="x", padx=8, pady=(8, 2))
        tk.Label(session_frame, text="Current Session",
                 font=("Courier New", 9, "bold"), bg="#12122a", fg=ACCENT,
                 anchor="w").pack(fill="x", padx=10, pady=(8, 2))
        self._session_preview = tk.Label(session_frame, text="Ask me anything...",
                 font=("Courier New", 7), bg="#12122a", fg=CHAT_MUTED,
                 anchor="w", wraplength=140, justify="left")
        self._session_preview.pack(fill="x", padx=10, pady=(0, 8))

        tk.Frame(parent, bg=CHAT_BORDER, height=1).pack(fill="x", padx=12, pady=4)

        # Quick action chips in sidebar
        tk.Label(parent, text="QUICK TOPICS",
                 font=("Courier New", 7, "bold"), bg=CHAT_SIDEBAR_BG, fg=CHAT_MUTED,
                 anchor="w").pack(fill="x", padx=12, pady=(8, 4))

        chips = [
            ("Available Books",  "available books"),
            ("Borrow Rules",     "borrow limit"),
            ("Fine & Fees",      "fine rules"),
            ("Get Recommendation","recommend a book"),
            ("How to Login",     "how do i login"),
            ("About System",     "about"),
        ]
        for label, query in chips:
            chip = tk.Frame(parent, bg=CHAT_SIDEBAR_BG, cursor="hand2")
            chip.pack(fill="x", padx=8, pady=2)
            dot = tk.Label(chip, text=">", font=("Courier New", 9), bg=CHAT_SIDEBAR_BG, fg=ACCENT2)
            dot.pack(side="left", padx=(8, 4))
            lbl = tk.Label(chip, text=label, font=("Courier New", 8), bg=CHAT_SIDEBAR_BG,
                           fg=TEXT, anchor="w", cursor="hand2")
            lbl.pack(side="left", fill="x", expand=True, pady=5)
            for w in (chip, dot, lbl):
                w.bind("<Button-1>", lambda e, q=query: self._chip_send(q))
                w.bind("<Enter>", lambda e, f=chip: f.config(bg="#11112a"))
                w.bind("<Leave>", lambda e, f=chip: f.config(bg=CHAT_SIDEBAR_BG))

        # Bottom info
        tk.Frame(parent, bg=CHAT_BORDER, height=1).pack(fill="x", padx=12, side="bottom", pady=4)
        tk.Label(parent, text="CIS SHELF  v2025\nDaniyal Ahmed",
                 font=("Courier New", 7), bg=CHAT_SIDEBAR_BG, fg=CHAT_MUTED,
                 justify="center").pack(side="bottom", pady=8)

    def _build_chat_area(self, parent):
        # Messages area
        msgs_outer = tk.Frame(parent, bg=CHAT_BG)
        msgs_outer.pack(fill="both", expand=True)

        self.msgs_canvas = tk.Canvas(msgs_outer, bg=CHAT_BG, highlightthickness=0)
        scroll = tk.Scrollbar(msgs_outer, orient="vertical",
                              command=self.msgs_canvas.yview,
                              bg=CHAT_BG, troughcolor="#0c0c1e", width=5)
        self.msgs_canvas.configure(yscrollcommand=scroll.set)
        scroll.pack(side="right", fill="y")
        self.msgs_canvas.pack(side="left", fill="both", expand=True)

        self.msgs_frame = tk.Frame(self.msgs_canvas, bg=CHAT_BG)
        self._canvas_window = self.msgs_canvas.create_window(
            (0, 0), window=self.msgs_frame, anchor="nw")

        self.msgs_frame.bind("<Configure>", self._on_msgs_configure)
        self.msgs_canvas.bind("<Configure>", self._on_canvas_configure)
        self.msgs_canvas.bind_all("<MouseWheel>",
            lambda e: self.msgs_canvas.yview_scroll(int(-1*(e.delta/120)), "units"))

        # Bottom input zone
        tk.Frame(parent, bg=CHAT_BORDER, height=1).pack(fill="x")
        input_zone = tk.Frame(parent, bg="#08081a")
        input_zone.pack(fill="x", padx=20, pady=14)

        # Suggestion row
        sugg_row = tk.Frame(input_zone, bg="#08081a")
        sugg_row.pack(fill="x", pady=(0, 8))
        for s in ["What books are available?", "How do I borrow?", "Tell me about fines"]:
            sb = tk.Button(sugg_row, text=s,
                           font=("Courier New", 7), bg="#12122a", fg=ACCENT2,
                           relief="flat", cursor="hand2", padx=8, pady=4,
                           activebackground=ACCENT2, activeforeground="white",
                           command=lambda t=s: self._chip_send(t))
            sb.pack(side="left", padx=(0, 6))

        # Input row
        input_row = tk.Frame(input_zone, bg="#08081a")
        input_row.pack(fill="x")

        input_wrap = tk.Frame(input_row, bg=CHAT_INPUT_BG,
                              highlightbackground=CHAT_BORDER, highlightthickness=1)
        input_wrap.pack(side="left", fill="x", expand=True)

        self.input_var = tk.StringVar()
        self.entry = tk.Entry(
            input_wrap, textvariable=self.input_var,
            bg=CHAT_INPUT_BG, fg=CHAT_TEXT, font=("Courier New", 11),
            insertbackground=ACCENT, relief="flat"
        )
        self.entry.pack(fill="x", ipady=11, padx=12)
        self.entry.bind("<Return>", lambda e: self._send())
        self.entry.bind("<FocusIn>",  lambda e: input_wrap.config(highlightbackground=ACCENT))
        self.entry.bind("<FocusOut>", lambda e: input_wrap.config(highlightbackground=CHAT_BORDER))
        self.entry.focus_set()

        send_btn = tk.Canvas(input_row, bg="#08081a", highlightthickness=0,
                             width=46, height=44, cursor="hand2")
        send_btn.pack(side="right", padx=(8, 0))
        self._draw_send_btn(send_btn)
        send_btn.bind("<Button-1>", lambda e: self._send())
        send_btn.bind("<Enter>",
            lambda e: (send_btn.delete("all"),
                       send_btn.create_rectangle(2,2,44,42,fill=ACCENT,outline="",tags="bg"),
                       send_btn.create_polygon(13,22, 34,14, 34,30, fill="#000010",tags="tri")))
        send_btn.bind("<Leave>",
            lambda e: self._draw_send_btn(send_btn))

    def _draw_send_btn(self, c):
        c.delete("all")
        c.create_rectangle(2, 2, 44, 42, fill=ACCENT2, outline="", tags="bg")
        c.create_polygon(13, 22, 34, 14, 34, 30, fill="white", tags="tri")

    # ── Canvas helpers ───────────────────────────────────────────────────────
    def _on_msgs_configure(self, e):
        self.msgs_canvas.configure(scrollregion=self.msgs_canvas.bbox("all"))

    def _on_canvas_configure(self, e):
        self.msgs_canvas.itemconfig(self._canvas_window, width=e.width)

    def _scroll_bottom(self):
        self.msgs_canvas.update_idletasks()
        self.msgs_canvas.yview_moveto(1.0)

    # ── Blinking dot ─────────────────────────────────────────────────────────
    def _blink_dot(self):
        try:
            self._dot_on = not self._dot_on
            self.dot_lbl.config(fg=SUCCESS if self._dot_on else "#003311")
            self.after(700, self._blink_dot)
        except Exception:
            pass

    # ── Message rendering ────────────────────────────────────────────────────
    def _add_message(self, role, text, is_typing=False):
        """role: 'user' | 'bot' | 'typing'"""
        ts = datetime.datetime.now().strftime("%H:%M")

        row = tk.Frame(self.msgs_frame, bg=CHAT_BG)
        row.pack(fill="x", padx=16, pady=6)

        if role == "user":
            # Right-aligned user bubble
            spacer = tk.Frame(row, bg=CHAT_BG)
            spacer.pack(side="left", fill="x", expand=True)

            bubble_col = tk.Frame(row, bg=CHAT_BG)
            bubble_col.pack(side="right")

            ts_lbl = tk.Label(bubble_col, text=f"You  {ts}",
                              font=("Courier New", 7), bg=CHAT_BG, fg=CHAT_MUTED, anchor="e")
            ts_lbl.pack(fill="x")

            bubble = tk.Frame(bubble_col, bg="#1a1a3d",
                              highlightbackground=ACCENT2, highlightthickness=1)
            bubble.pack(anchor="e", pady=(2, 0))

            tk.Label(bubble, text=text, font=("Courier New", 10), bg="#1a1a3d",
                     fg=TEXT, wraplength=340, justify="left",
                     padx=14, pady=10).pack()

            # Update sidebar preview
            preview = text[:35] + ("..." if len(text) > 35 else "")
            try:
                self._session_preview.config(text=preview)
            except Exception:
                pass

        elif role == "bot":
            # Left-aligned bot bubble
            bubble_col = tk.Frame(row, bg=CHAT_BG)
            bubble_col.pack(side="left")

            # Avatar + label
            meta = tk.Frame(bubble_col, bg=CHAT_BG)
            meta.pack(fill="x")
            avt = tk.Label(meta, text="AI",
                           font=("Courier New", 7, "bold"),
                           bg=ACCENT2, fg="white",
                           width=3, pady=2)
            avt.pack(side="left")
            tk.Label(meta, text=f"  CIS SHELF  {ts}",
                     font=("Courier New", 7), bg=CHAT_BG, fg=CHAT_MUTED).pack(side="left")

            bubble = tk.Frame(bubble_col, bg="#0d1020",
                              highlightbackground=CHAT_BORDER, highlightthickness=1)
            bubble.pack(anchor="w", pady=(4, 0))

            tk.Label(bubble, text=text, font=("Courier New", 10),
                     bg="#0d1020", fg=SUCCESS,
                     wraplength=340, justify="left",
                     padx=14, pady=10).pack()

            spacer = tk.Frame(row, bg=CHAT_BG)
            spacer.pack(side="right", fill="x", expand=True)

        elif role == "typing":
            # Typing indicator
            bubble_col = tk.Frame(row, bg=CHAT_BG)
            bubble_col.pack(side="left")

            bubble = tk.Frame(bubble_col, bg="#0d1020",
                              highlightbackground=CHAT_BORDER, highlightthickness=1)
            bubble.pack(anchor="w")

            self._typing_lbl = tk.Label(bubble, text="CIS SHELF is thinking...",
                                        font=("Courier New", 9, "italic"),
                                        bg="#0d1020", fg=CHAT_MUTED,
                                        padx=14, pady=10)
            self._typing_lbl.pack()
            self._animate_typing(0)

        self._current_row = row if role == "typing" else None
        self._scroll_bottom()
        return row

    def _animate_typing(self, step):
        try:
            dots = "." * (step % 4)
            self._typing_lbl.config(text=f"CIS SHELF is thinking{dots}")
            self.after(400, lambda: self._animate_typing(step + 1))
        except Exception:
            pass

    def _remove_typing_row(self):
        try:
            if self._current_row:
                self._current_row.destroy()
                self._current_row = None
        except Exception:
            pass

    # ── Send logic ───────────────────────────────────────────────────────────
    def _chip_send(self, text):
        self.input_var.set(text)
        self._send()

    def _send(self):
        msg = self.input_var.get().strip()
        if not msg or self._typing:
            return
        self.input_var.set("")
        self._typing = True
        self._add_message("user", msg)
        self._add_message("typing", "")
        threading.Thread(target=self._reply_thread, args=(msg,), daemon=True).start()

    def _reply_thread(self, msg):
        try:
            import time
            time.sleep(0.7)
            response = get_bot_response(msg)
            try:
                self.after(0, self._remove_typing_row)
                self.after(80, lambda: self._add_message("bot", response))
                self.after(80, lambda: setattr(self, "_typing", False))
            except Exception:
                pass
        except Exception:
            try:
                self.after(0, self._remove_typing_row)
                self.after(80, lambda: self._add_message("bot", "Something went wrong. Please try again."))
                self.after(80, lambda: setattr(self, "_typing", False))
            except Exception:
                pass

    # ── Welcome message ──────────────────────────────────────────────────────
    def _post_welcome(self):
        welcome = (
            "Hello! Welcome to the CIS SHELF AI Assistant.\n\n"
            "I can help you with:\n"
            "   Books and recommendations\n"
            "   Borrow rules and limits\n"
            "   Fine and balance information\n"
            "   Registration and login help\n\n"
            "Type your question below or click a topic on the left!"
        )
        self.after(100, lambda: self._add_message("bot", welcome))


# ===================== MAIN APP =====================
class LibraryApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("CIS SHELF — E-Library System")
        self.geometry("540x720")
        self.resizable(False, False)
        self.configure(bg=BG)
        self.current_frame = None
        self._build_navbar()
        self.show_welcome()

    def _build_navbar(self):
        self.navbar = tk.Canvas(self, bg=BG, highlightthickness=0, height=48, width=540)
        self.navbar.place(x=0, y=0)
        self._navbar_offset = 0
        self._draw_navbar()
        self._animate_navbar()

    def _draw_navbar(self):
        self.navbar.delete("all")
        w, h = 540, 48
        self.navbar.create_rectangle(0, 0, w, h, fill="#09091a", outline="")
        dot_spacing = 60
        offset = int(self._navbar_offset) % dot_spacing
        for x in range(-dot_spacing+offset, w+dot_spacing, dot_spacing):
            for y_pos in [8, 24, 38]:
                alpha = 0.3 + 0.15*math.sin((x+y_pos+self._navbar_offset)*0.05)
                brightness = int(alpha*40)
                col = f"#{brightness:02x}{brightness:02x}{min(brightness*2,0xff):02x}"
                self.navbar.create_oval(x-1,y_pos-1,x+1,y_pos+1,fill=col,outline="")
        pulse = 0.5 + 0.5*math.sin(self._navbar_offset*0.08)
        g = int(150+79*pulse); b = int(180+75*pulse)
        self.navbar.create_rectangle(0,h-3,w,h,fill=f"#00{g:02x}{b:02x}",outline="")
        self.navbar.create_rectangle(0,h-6,int(w*(0.4+0.1*pulse)),h-3,fill=ACCENT2,outline="")
        self.navbar.create_text(28,h//2-1,text="[*]",font=("Courier New",12,"bold"),fill=ACCENT,anchor="w")
        self.navbar.create_text(62,h//2,text="CIS",font=("Courier New",14,"bold"),fill=ACCENT,anchor="w")
        self.navbar.create_text(100,h//2,text="SHELF",font=("Courier New",14,"bold"),fill=ACCENT2,anchor="w")
        blink = self._navbar_offset % 60 < 30
        self.navbar.create_oval(490,h//2-5,500,h//2+5,fill=SUCCESS if blink else "#004d22",outline="")
        self.navbar.create_text(510,h//2,text="LIVE",font=("Courier New",7,"bold"),fill=SUCCESS,anchor="w")

    def _animate_navbar(self):
        self._navbar_offset += 1
        self._draw_navbar()
        self.after(30, self._animate_navbar)

    def switch_frame(self, frame_class):
        if self.current_frame:
            self.current_frame.destroy()
        self.current_frame = frame_class(self)
        self.current_frame.place(x=0, y=48, width=540, height=672)

    def show_welcome(self):
        self.switch_frame(WelcomePage)

    def show_register(self):
        self.switch_frame(RegisterPage)

    def show_chat(self):
        self.switch_frame(ChatPage)


# ===================== WELCOME PAGE =====================
class WelcomePage(tk.Frame):
    def __init__(self, master):
        super().__init__(master, bg=BG)
        self._build()

    def _build(self):
        bg_c = tk.Canvas(self, bg=BG, highlightthickness=0, width=540, height=672)
        bg_c.place(x=0, y=0)
        for i in range(0, 540, 36):
            bg_c.create_line(i, 0, i, 672, fill="#0e0e22", width=1)
        for i in range(0, 672, 36):
            bg_c.create_line(0, i, 540, i, fill="#0e0e22", width=1)

        tk.Label(self, text="[LIB]", font=("Courier New", 36, "bold"), bg=BG, fg=ACCENT).pack(pady=(50,4))
        tk.Label(self, text="CIS SHELF", font=("Courier New", 30, "bold"),
                 bg=BG, fg=ACCENT).pack()
        tk.Label(self, text="E - L I B R A R Y   S Y S T E M",
                 font=("Courier New", 10), bg=BG, fg=ACCENT2).pack(pady=(2,0))

        tk.Frame(self, bg=ACCENT, height=2, width=220).pack(pady=16)
        tk.Label(self, text="Your Digital Library, Reimagined.",
                 font=("Courier New", 9), bg=BG, fg=MUTED).pack()

        stats = tk.Frame(self, bg=CARD_BG, bd=0)
        stats.pack(pady=24, padx=50, fill="x")
        for icon, label, val in [("[B]","Books","5+"),("[U]","Users","inf"),("[F]","Speed","Fast")]:
            col = tk.Frame(stats, bg=CARD_BG)
            col.pack(side="left", expand=True, pady=14)
            tk.Label(col, text=icon, font=("Courier New",14,"bold"), bg=CARD_BG, fg=ACCENT2).pack()
            tk.Label(col, text=val, font=("Courier New",14,"bold"), bg=CARD_BG, fg=ACCENT).pack()
            tk.Label(col, text=label, font=("Courier New",8), bg=CARD_BG, fg=MUTED).pack()

        btn_frame = tk.Frame(self, bg=BG)
        btn_frame.pack(pady=6, padx=50, fill="x")

        BorderAnimButton(btn_frame, text="REGISTER NEW USER",
            command=self.master.show_register,
            btn_bg=CARD_BG, btn_fg=ACCENT,
            font_spec=("Courier New",11,"bold"),
            height=50, width=440, border_color=ACCENT
        ).pack(pady=(0,10))

        BorderAnimButton(btn_frame, text="CIS SHELF ASSISTANT",
            command=self.master.show_chat,
            btn_bg=CARD_BG, btn_fg=ACCENT2,
            font_spec=("Courier New",11,"bold"),
            height=50, width=440, border_color=ACCENT2
        ).pack(pady=(0,10))

        BorderAnimButton(btn_frame, text="EXIT",
            command=self.master.destroy,
            btn_bg="#0a0a18", btn_fg=MUTED,
            font_spec=("Courier New",11),
            height=50, width=440, border_color=ACCENT2
        ).pack()

        tk.Label(self, text="2025 CIS SHELF  |  Daniyal Ahmed",
                 font=("Courier New",8), bg=BG, fg=BORDER).pack(side="bottom", pady=12)


# ===================== REGISTER PAGE =====================
class RegisterPage(tk.Frame):
    def __init__(self, master):
        super().__init__(master, bg=BG)
        self.entries    = {}
        self.show_pass  = {}
        self.status_var = tk.StringVar()
        self._build()

    def _build(self):
        header = tk.Frame(self, bg=CARD_BG)
        header.pack(fill="x")
        inner = tk.Frame(header, bg=CARD_BG)
        inner.pack(pady=14, padx=20, fill="x")

        BorderAnimButton(inner, text="BACK",
            command=self.master.show_welcome,
            btn_bg=CARD_BG, btn_fg=MUTED,
            font_spec=("Courier New",9),
            height=32, width=90, border_color=ACCENT2
        ).pack(side="left")

        tk.Label(inner, text="NEW REGISTRATION",
                 font=("Courier New",13,"bold"), bg=CARD_BG, fg=ACCENT).pack(side="right")

        canvas = tk.Canvas(self, bg=BG, highlightthickness=0)
        canvas.pack(fill="both", expand=True)
        scrollbar = tk.Scrollbar(self, orient="vertical", command=canvas.yview,
                                 bg=BG, troughcolor=BORDER, width=6)
        canvas.configure(yscrollcommand=scrollbar.set)

        form_container = tk.Frame(canvas, bg=BG)
        canvas_win = canvas.create_window((0,0), window=form_container, anchor="nw")

        def on_configure(e):
            canvas.configure(scrollregion=canvas.bbox("all"))
            canvas.itemconfig(canvas_win, width=canvas.winfo_width())
        form_container.bind("<Configure>", on_configure)

        form = tk.Frame(form_container, bg=BG)
        form.pack(padx=30, pady=8, fill="x")

        for label, key, is_pass in [
            ("USERNAME","username",False), ("PASSWORD","password",True),
            ("FIRST NAME","firstname",False), ("LAST NAME","lastname",False),
            ("ADDRESS","address",False), ("EMAIL","email",False)
        ]:
            self._make_field(form, label, key, is_pass)

        self.status_lbl = tk.Label(form_container, textvariable=self.status_var,
                                   font=("Courier New",9), bg=BG, fg=SUCCESS, wraplength=460)
        self.status_lbl.pack(pady=(4,0))

        BorderAnimButton(form_container, text="CIS SHELF ASSISTANT",
            command=self.master.show_chat,
            btn_bg=CARD_BG, btn_fg=ACCENT2,
            font_spec=("Courier New",11,"bold"),
            height=52, width=480, border_color=ACCENT2
        ).pack(pady=(10,4))

        self.reg_btn = BorderAnimButton(form_container,
            text="COMPLETE REGISTRATION",
            command=self._on_register,
            btn_bg=CARD_BG, btn_fg=ACCENT,
            font_spec=("Courier New",11,"bold"),
            height=52, width=480, border_color=ACCENT
        )
        self.reg_btn.pack(pady=10)

        info = tk.Frame(form_container, bg=CARD_BG)
        info.pack(padx=30, pady=6, fill="x")
        tk.Label(info,
                 text="After registration the Library System opens automatically.\n"
                      "A confirmation email with your PDF report will be sent.",
                 font=("Courier New",8), bg=CARD_BG, fg=MUTED,
                 wraplength=440, justify="left").pack(padx=12, pady=10)

        def _on_mousewheel(event):
            canvas.yview_scroll(int(-1*(event.delta/120)), "units")
        canvas.bind_all("<MouseWheel>", _on_mousewheel)

    def _make_field(self, parent, label_text, key, is_pass):
        wrapper = tk.Frame(parent, bg=BG)
        wrapper.pack(fill="x", pady=5)
        tk.Label(wrapper, text=label_text, font=("Courier New",8,"bold"),
                 bg=BG, fg=MUTED, anchor="w").pack(fill="x")
        entry_frame = tk.Frame(wrapper, bg=INPUT_BG,
                               highlightbackground=BORDER, highlightthickness=1)
        entry_frame.pack(fill="x")
        icons = {"username":">","password":"*","firstname":"-",
                 "lastname":"-","address":"@","email":"~"}
        tk.Label(entry_frame, text=icons.get(key,">"),
                 font=("Courier New",10), bg=INPUT_BG, fg=ACCENT, width=2
                 ).pack(side="left", padx=(8,0))
        entry = tk.Entry(entry_frame, show="*" if is_pass else "",
                         font=("Courier New",11), bg=INPUT_BG, fg=TEXT,
                         insertbackground=ACCENT, relief="flat", bd=0)
        entry.pack(side="left", fill="x", expand=True, ipady=9, padx=(4,4))
        if is_pass:
            self.show_pass[key] = False
            eye_lbl = tk.Label(entry_frame, text="[show]", font=("Courier New",8),
                               bg=INPUT_BG, fg=MUTED, cursor="hand2")
            eye_lbl.pack(side="right", padx=(0,8))
            def toggle_pass(e, k=key, en=entry, lbl=eye_lbl):
                self.show_pass[k] = not self.show_pass[k]
                en.config(show="" if self.show_pass[k] else "*")
                lbl.config(fg=ACCENT if self.show_pass[k] else MUTED, text="[hide]" if self.show_pass[k] else "[show]")
            eye_lbl.bind("<Button-1>", toggle_pass)
        entry.bind("<FocusIn>",  lambda e, f=entry_frame: f.config(highlightbackground=ACCENT))
        entry.bind("<FocusOut>", lambda e, f=entry_frame: f.config(highlightbackground=BORDER))
        self.entries[key] = entry

    def _on_register(self):
        username  = self.entries["username"].get().strip()
        password  = self.entries["password"].get().strip()
        firstname = self.entries["firstname"].get().strip()
        lastname  = self.entries["lastname"].get().strip()
        address   = self.entries["address"].get().strip()
        email     = self.entries["email"].get().strip()

        if not all([username, password, firstname, lastname, address, email]):
            messagebox.showerror("Missing Fields", "All fields are required!")
            return
        if "@" not in email:
            messagebox.showerror("Invalid Email", "Please enter a valid email address!")
            return

        self.reg_btn.set_text("PROCESSING...")
        self.reg_btn.set_state("disabled")
        self.status_var.set("Saving registration...")
        self.update()

        def process():
            try:
                save_user_to_file(username, password, firstname, lastname, address, email)
                self.status_var.set("User saved  |  Generating PDF...")
                self.update()
                pdf_file = generate_pdf(username, firstname, lastname, address, email)
                self.status_var.set("PDF ready  |  Sending email...")
                self.update()
                try:
                    send_email_func(pdf_file, firstname, email)
                    self.status_var.set("Email sent successfully!")
                except Exception:
                    self.status_var.set("Email failed (registration still saved)")
                self.update()
                messagebox.showinfo("Success",
                    f"Welcome {firstname}!\nRegistration complete.\nThe library system will now open.")
                self.master.destroy()
                if os.path.exists(CPP_EXE):
                    subprocess.Popen(["cmd", "/c", "start", "", CPP_EXE])
                else:
                    messagebox.showerror("Error", f"Could not find:\n{CPP_EXE}")
            except Exception as e:
                self.reg_btn.set_state("normal")
                self.reg_btn.set_text("COMPLETE REGISTRATION")
                messagebox.showerror("Error", str(e))

        threading.Thread(target=process, daemon=True).start()


# ===================== MAIN =====================
if __name__ == "__main__":
    app = LibraryApp()
    speak("Welcome to the AI powered CIS SHELF")
    app.mainloop()