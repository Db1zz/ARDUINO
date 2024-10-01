// JavaScript Code to handle the color selection and apply to grid buttons

// Variable to hold the selected color
let selectedColor = '';

// Get all the color buttons
const colorButtons = document.querySelectorAll('.color-btn');

// Get all the grid buttons
const gridButtons = document.querySelectorAll('.grid-btn');

// Add event listeners to the color buttons
colorButtons.forEach(button => {
    button.addEventListener('click', function() {
        // Get the data-color attribute from the clicked button
        selectedColor = this.getAttribute('data-color');

        // Highlight the selected color button
        colorButtons.forEach(btn => btn.classList.remove('active-color'));
        this.classList.add('active-color');
    });
});

// Add event listeners to the grid buttons
gridButtons.forEach(button => {
    button.addEventListener('click', function() {
        // Change the button background to the selected color
        if (selectedColor) {
            this.style.backgroundColor = selectedColor;
        } else {
            alert("Please select a color first.");
        }
    });
});
