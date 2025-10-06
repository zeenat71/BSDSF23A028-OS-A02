cat > REPORT.md << EOF
### Feature 3: Column Display (Down Then Across)

**Q1. Explain the general logic for printing items in a "down then across" columnar format. Why is a simple single loop through the list of filenames insufficient for this task?**  

- General Logic:  
  1. Pehle **sab filenames ek dynamically allocated array me store** karte hain.  
  2. Maximum filename length nikalte hain aur spacing add karte hain.  
  3. Terminal width ko use karke **number of columns** aur **rows** calculate karte hain:  
     columns = terminal_width / (max_filename_length + spacing)  
     rows = ceil(total_files / columns)  
  4. Print karte waqt **row-wise iteration** karte hain:  
     - First row: filenames[0], filenames[0 + rows], filenames[0 + 2*rows] …  
     - Second row: filenames[1], filenames[1 + rows], filenames[1 + 2*rows] …  
  - Is tarah se items "down then across" print hote hain.  

- Reason single loop is insufficient:  
  - Simple loop se files sirf **row-wise ya column-wise sequentially** print hongi.  
  - Columns align nahi honge, aur output ls ke default style jaisa nahi hoga.  

**Q2. What is the purpose of the ioctl system call in this context? What would be the limitations of your program if you only used a fixed-width fallback (e.g., 80 columns) instead of detecting the terminal size?**  

- Purpose of ioctl:
  - `ioctl` aur `TIOCGWINSZ` ka use **current terminal width** detect karne ke liye hota hai.  
  - Isse program automatically **columns adjust** karta hai according to terminal size.  

- Limitation of fixed-width fallback (80 columns):  
  - Agar terminal chhota hai → output overflow ho sakta hai aur columns align nahi honge.  
  - Agar terminal bada hai → unnecessary empty space hogi.  
  - User experience poor ho jayega; ls ka default behavior accurately mimic nahi hoga.  

EOF
