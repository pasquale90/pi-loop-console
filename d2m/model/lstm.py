#@title simple lstm
class simplelstm(nn.Module):
    def __init__(self, input_dim, hidden_dim, seq_len, num_layers=2, batch_size=16, output_dim=50 ):
        super(simplelstm, self).__init__()
        self.input_dim = input_dim
        self.hidden_dim = hidden_dim
        self.batch_size = batch_size
        self.num_layers = num_layers
        self.seq_len = seq_len

        # setup LSTM layer
        self.lstm = nn.LSTM(self.input_dim, self.hidden_dim, self.num_layers, batch_first=True)

        # setup output layer
        self.fc = nn.Linear(self.hidden_dim*2, output_dim)

    def init_hidden(self):
        return (
            torch.zeros(self.num_layers, self.batch_size, self.hidden_dim),
            torch.zeros(self.num_layers, self.batch_size, self.hidden_dim),
        )

    def forward(self, x):
        print('input',x.shape)
        x = x.view(x.shape[0],431,80)
        print('squeeze',x.shape)
        h0 = Variable(torch.zeros(self.num_layers, x.size(0), self.hidden_dim).float())
        #print('h0',h0.shape)
        c0 = Variable(torch.zeros(self.num_layers, x.size(0), self.hidden_dim).float())
        #print('c0',c0.shape)

        x, _ = self.lstm(x,(h0,c0))
        print('lstm_out',x.shape)
        x = self.fc(x[:, -1, :])
        print('fc',x.shape)
        #logits = self.linear(x[:, -1, :])#lstm_out[-1]
        #print('logits',logits.shape)        
        #x = x.squeeze(1)

        #genre_scores = F.log_softmax(x, dim=1)
        #print('softmax',genre_scores.shape)
        return genre_scores
