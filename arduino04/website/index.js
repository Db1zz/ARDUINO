let selectedColor = ''; // Variable to track selected color

// Set up event listeners for color buttons
document.querySelectorAll('.color-btn').forEach(button => {
    button.addEventListener('click', function() {
        const currentColor = this.getAttribute('data-color');
        
        if (currentColor === 'RESET') {
            // Reset all grid buttons to white when RESET is clicked
            document.querySelectorAll('.grid-btn').forEach(btn => {
                btn.style.backgroundColor = 'white'; // Reset grid button color to white
            });

            // Send request to ESP8266 to reset all LEDs (turn off all)
            fetch('/resetMatrix', { method: 'POST' })
            .then(response => response.text())
            .then(result => console.log(result)) // Log response
            .catch(error => console.error('Error:', error));

        } else {
            // Toggle color selection: If this button is clicked again, deselect it
            if (selectedColor === currentColor) {
                selectedColor = ''; // Deselect the color
                console.log('Deselected Color: ' + currentColor);
                this.classList.remove('active-color'); // Remove highlight from the button
            } else {
                selectedColor = currentColor; // Select the new color
                console.log('Selected Color: ' + selectedColor);
                
                // Highlight the selected color button and remove highlight from others
                document.querySelectorAll('.color-btn').forEach(btn => btn.classList.remove('active-color'));
                this.classList.add('active-color'); // Highlight the current button
            }
        }
    });
});

// Set up event listeners for grid buttons
document.querySelectorAll('.grid-btn').forEach((button, index) => {
    button.addEventListener('click', function() {
        // If a color is selected, set the button color; if not, set it back to white
        let colorToSet = selectedColor ? selectedColor : 'white';

        // Change the button's color on the webpage
        this.style.backgroundColor = colorToSet;

        // Prepare data to send to the ESP8266
        const data = new URLSearchParams();
        data.append('color', colorToSet);
        data.append('button', index); // Send the grid button index

        // Send POST request to ESP8266
        fetch('/setColor', {
            method: 'POST',
            body: data
        })
        .then(response => response.text())
        .then(result => console.log(result)) // Print ESP8266 response
        .catch(error => console.error('Error:', error));
    });
});
