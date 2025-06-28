package org.cpd2425.t09g13;

import org.cpd2425.t09g13.protocol.Message;
import org.cpd2425.t09g13.room.DataMsg;
import org.cpd2425.t09g13.room.RoomMsg;
import org.cpd2425.t09g13.sync.channel.InvalidReceiverException;
import org.cpd2425.t09g13.sync.channel.Receiver;
import org.cpd2425.t09g13.sync.channel.Sender;
import org.json.JSONObject;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.Optional;

public class Ollama {
    private final URI uri;
    private final HttpClient httpClient;
    private final String initialPrompt;
    private final Sender<RoomMsg> sender;
    private Receiver<RoomMsg> receiver;
    private boolean closed;

    public Ollama(String initialPrompt, Sender<RoomMsg> room) {
        try {
            this.httpClient = HttpClient.newHttpClient();
            this.uri = URI.create("http://localhost:11434/api/generate");
        } catch (Exception e) {
            throw new RuntimeException("Failed to initialize URL", e);
        }
        this.initialPrompt = initialPrompt;
        this.sender = room;
        this.receiver = room.newReceiver();
        this.closed = false;
    }

    private String send(String prompt) {
        JSONObject message = new JSONObject();
        message.put("model", "llama3.2:1b");
        message.put("prompt", this.initialPrompt + "\n\n" + prompt);
        message.put("stream", false);

        HttpRequest request = HttpRequest.newBuilder()
                .uri(this.uri)
                .POST(HttpRequest.BodyPublishers.ofString(message.toString()))
                .header("Content-Type", "application/json")
                .build();

        try {
            HttpResponse<String> response = this.httpClient.send(request, HttpResponse.BodyHandlers.ofString());
            JSONObject json = new JSONObject(response.body());
            return json.getString("response");
        } catch (Exception ignored) {}
        return "";
    }

    private void messageHandler() {
        while (!this.closed) {
            this.receiver.waitForMsg();
            try {
                Optional<RoomMsg> _msg = this.receiver.receive();
                for (; _msg.isPresent(); _msg = this.receiver.receive()) {
                    if (_msg.get() instanceof DataMsg(Message message)) {
                        if (!message.author().equals("AI") && !message.author().equals("Server")) {
                            String answer = this.send(message.msg());
                            this.sender.send(new DataMsg(new Message(
                                    "AI",
                                    "To " + message.author() + ":\n" + answer
                            )));
                        }
                    }
                }
            } catch (InvalidReceiverException e) {
                this.sender.send(new DataMsg(new Message(
                        "AI",
                        "WARNING: Could not handle all the requests. Restarting AI..."
                )));
                this.receiver = this.sender.newReceiver();
            }
        }
    }

    public void start() {
        Thread.ofVirtual().start(this::messageHandler);
    }

    public void stop() {
        this.closed = true;
    }

    public static void configureModel() {
        try {
            HttpClient client = HttpClient.newHttpClient();
            URI uri = URI.create("http://localhost:11434/api/pull");

            JSONObject requestMsg = new JSONObject();
            requestMsg.put("model", "llama3.2:1b");
            requestMsg.put("stream", false);

            HttpRequest request = HttpRequest.newBuilder()
                    .uri(uri)
                    .POST(HttpRequest.BodyPublishers.ofString(requestMsg.toString()))
                    .header("Content-Type", "application/json")
                    .build();

            HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
            JSONObject responseMsg = new JSONObject(response.body());
            if (!responseMsg.getString("status").equals("success")) {
                throw new RuntimeException("Status: " + responseMsg.getString("status"));
            }
            client.close();
        } catch (Exception e) {
            throw new RuntimeException("Failed to pull model.", e);
        }
    }
}
