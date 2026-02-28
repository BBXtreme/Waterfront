const express = require('express');
const bodyParser = require('body-parser');
const { createClient } = require('@supabase/supabase-js');
const mqtt = require('mqtt');

// Initialize Express app
const app = express();
app.use(bodyParser.json());

// Initialize Supabase client with env vars
const supabaseUrl = process.env.SUPABASE_URL;
const supabaseKey = process.env.SUPABASE_KEY;
const supabase = createClient(supabaseUrl, supabaseKey);

// MQTT client
const mqttBroker = process.env.MQTT_BROKER || 'mqtt://localhost:1883';
const mqttClient = mqtt.connect(mqttBroker);

mqttClient.on('connect', () => {
  console.log('MQTT connected');
});

mqttClient.on('error', (err) => {
  console.error('MQTT error:', err);
});

// Basic route for testing
app.get('/', (req, res) => {
  res.send('Waterfront Backend is running');
});

// Start server
const port = process.env.PORT || 3000;
app.listen(port, () => {
  console.log(`Backend on ${port}`);
});
