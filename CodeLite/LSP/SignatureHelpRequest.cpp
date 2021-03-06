#include "LSP/LSPEvent.h"
#include "SignatureHelpRequest.h"

LSP::SignatureHelpRequest::SignatureHelpRequest(const wxFileName& filename, size_t line, size_t column)
    : m_filename(filename)
    , m_line(line)
    , m_column(column)
{
    SetMethod("textDocument/signatureHelp");
    m_params.reset(new TextDocumentPositionParams());
    m_params->As<TextDocumentPositionParams>()->SetTextDocument(TextDocumentIdentifier(filename.GetFullPath()));
    m_params->As<TextDocumentPositionParams>()->SetPosition(Position(line, column));
}

LSP::SignatureHelpRequest::~SignatureHelpRequest() {}

void LSP::SignatureHelpRequest::OnResponse(const LSP::ResponseMessage& response, wxEvtHandler* owner,
                                           IPathConverter::Ptr_t pathConverter)
{
    if(!response.Has("result")) {
        return;
    }
    JSONItem res = response.Get("result");
    LSP::SignatureHelp sh;
    sh.FromJSON(res, pathConverter);

    LSPEvent event(wxEVT_LSP_SIGNATURE_HELP);
    event.SetSignatureHelp(sh);
    owner->AddPendingEvent(event);
}

bool LSP::SignatureHelpRequest::IsValidAt(const wxFileName& filename, size_t line, size_t col) const
{
    return (m_filename == filename) && (m_line == line) && (m_column == col);
}
