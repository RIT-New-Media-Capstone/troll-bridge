const express = require('express');
const axios = require('axios');
require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 3000;
const API_KEY = "Insert API Key when recieved";

app.get('/trivia', async (req, res) => {
    try {
        const response = await axios.post(
            'https://api.openai.com/v1/completions',
            {
                model: 'gpt-3.5-turbo',
                prompt: 'Generate a JSON array of 10 "true or false" trivia questions with their answers formatted by "question" object (string) and "answer" object (boolean).',
                max_tokens: 10,
                n: 1,
                stop: null,
                temperature: 0.7
            },
            {
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${process.env.API_KEY}`
                }
            }
        );

        const triviaData = response.data.choices[0].text.trim();

        res.json(JSON.parse(triviaData));

        /*Add the serialport functionality here*/

    } catch (error) {
        console.error('Error fetching trivia data:', error);
        res.status(500).json({ error: 'Failed to fetch trivia data' });
    }
});

app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});