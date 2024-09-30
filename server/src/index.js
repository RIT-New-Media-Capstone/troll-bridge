// npm packages
const express = require('express');
const OpenAI = require('openai');
require('dotenv').config();

if (!process.env.API_KEY) {
    console.log("ERROR: NO API KEY FOUND IN .ENV");
    return;
}

const API_KEY = process.env.API_KEY;

// Create openai component
const openai = new OpenAI({
    apiKey: API_KEY,
    organization: "org-C0V9tT5zjNoT3VLZp6IH4mwF",
    project: "proj_kc8w1NtSPp6EjNPH8hGKComP"
});

// Create app and port
const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.json());

// API call and handling responser
app.get('/trivia', async (req, res) => {
    try {
        // Call openai using openai npm
        const response = await openai.chat.completions.create({
            model: "gpt-4o-mini",
            messages: [
                { role: "system", content: "You are a trivia game creator." },
                { role: "user", content: "Please generate 10 trivia questions and their corresponding answers in JSON format. The questions must be true or false, and the answer must be a boolean value. Respond with nothing other thatn the json, no words or text." }
            ],
            max_tokens: 500
        });

        let triviaData;

        // Parse String response
        const triviaResponse = response.choices[0].message.content;
        let responseString = triviaResponse.replace(/```json|```/g, '');
        responseString = responseString.replace(/\n/g, '').trim();

        // Parse JSON
        try {
            triviaData = JSON.parse(responseString);
        } catch (error) {
            triviaData = { error: 'Failed to parse JSON', response: responseString };
        }

        // Testing
        res.json(triviaData);
        /*
        // Fill question and answer arrays
        let questionArray=[];
        let answerArray = [];

        triviaData.forEach(element => {
            questionArray.push(element.question);
            answerArray.push(element.answer);
        });

        // Testing
        console.log(questionArray);
        console.log(answerArray);

        // Write to serial
        writeToSerial(questionArray.join());
        writeToSerial(answerArray.join());*/

    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Error fetching trivia questions' });
    }
});

// Hosting
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});